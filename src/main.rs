use std::sync::Arc;

use eframe::egui::{CentralPanel, Color32, FontData, FontDefinitions, FontFamily, Id, Panel, RichText, ScrollArea, Sense, Slider, Ui, Vec2, ecolor::Hsva};
use egui_plot::{Line, Plot, PlotBounds, PlotPoints};

mod player;
mod strategy;

#[cfg(not(target_arch = "wasm32"))]
fn main() -> eframe::Result {
    eframe::run_native(
        "Prisoner's dilemma",
        Default::default(),
        Box::new(|cc| Ok(Box::new(DilemmaApp::new(cc)))),
    )
}

#[cfg(target_arch = "wasm32")]
fn main() {
    use eframe::wasm_bindgen::JsCast as _;

    let web_options = eframe::WebOptions::default();
    wasm_bindgen_futures::spawn_local(async {
        let document = web_sys::window()
            .expect("No window")
            .document()
            .expect("No document");

        let canvas = document
            .get_element_by_id("canvas_id")
            .expect("Failed to find canvas_id")
            .dyn_into::<web_sys::HtmlCanvasElement>()
            .expect("canvas_id was not a HtmlCanvasElement");

        let start_result = eframe::WebRunner::new()
            .start(
                canvas,
                web_options,
                Box::new(|cc| Ok(Box::new(DilemmaApp::new(cc)))),
            )
            .await;

        // Remove the loading text and spinner:
        if let Some(loading_text) = document.get_element_by_id("loading_text") {
            match start_result {
                Ok(_) => loading_text.remove(),
                Err(e) => {
                    loading_text.set_inner_html("Cannot start the app");
                    panic!("Failed to start eframe: {e:?}");
                }
            }
        }
    });
}

const ROUNDS: usize = 100;
const DEFAULT_RATING: [[i32; 2]; 2] = [[1, 5], [0, 3]];

struct DilemmaApp {
    rating: [[i32; 2]; 2],
    counts: Vec<usize>,
    last_game: Option<([[i32; 2]; 2], Vec<usize>, Vec<player::Player>)>,
}

impl Default for DilemmaApp {
    fn default() -> Self {
        Self {
            rating: DEFAULT_RATING,
            counts: vec![1; player::all_players().len()],
            last_game: None,
        }
    }
}

fn gen_color(index: usize) -> Color32 {
    const PHI: f32 = 1.618_033_988_749_894;
    const SV: [(f32, f32); 3] = [(1.0, 1.0), (1.0, 0.5), (0.5, 1.0)];

    let h = (index as f32 / 6.0 / PHI).fract(); 
    let (s, v) = SV[index % SV.len()];
    Color32::from(Hsva::new(h, s, v, 1.0))
}

impl DilemmaApp {
    fn new(cc: &eframe::CreationContext) -> Self {
        // Only the one font we need, the default ones are huge
        let mut fonts = FontDefinitions::empty();
        fonts.font_data.insert("Ubuntu-Light".to_owned(), Arc::new(FontData::from_static(epaint_default_fonts::UBUNTU_LIGHT)));
        for family in [FontFamily::Proportional, FontFamily::Monospace] {
            fonts.families.insert(family, vec!["Ubuntu-Light".to_owned()]);
        }
        cc.egui_ctx.set_fonts(fonts);
        Self::default()
    }

    fn options_ui(&mut self, ui: &mut Ui) {
        ui.horizontal(|ui| {
            ui.heading("Payoffs");
            if ui.small_button("Reset").clicked() {
                self.rating = DEFAULT_RATING;
            }
        });
        ui.label(RichText::new("Points a player gets in one round").weak());
        ui.add_space(4.0);

        let payoffs = [
            ((1, 1), "Both players cooperate"),
            ((0, 1), "Player betrays an opponent who cooperates"),
            ((1, 0), "Player cooperates but gets betrayed"),
            ((0, 0), "Both players betray each other"),
        ];
        for ((me, opponent), text) in payoffs {
            ui.label(text);
            ui.add(Slider::new(&mut self.rating[me][opponent], 0..=10).suffix(" points"));
            ui.add_space(6.0);
        }

        ui.add_space(12.0);
        ui.horizontal(|ui| {
            ui.heading("Players");
            if ui.small_button("Reset").clicked() {
                self.counts.fill(1);
            }
        });
        ui.label(RichText::new("How many players use each strategy").weak());
        ui.add_space(4.0);

        for p in player::all_players() {
            ui.separator();
            ui.horizontal(|ui| {
                let (rect, _) = ui.allocate_exact_size(Vec2::splat(12.0), Sense::hover());
                ui.painter().rect_filled(rect, 2.0, gen_color(p.index()));
                ui.label(RichText::new(p.name()).strong());
            });
            ui.add(Slider::new(&mut self.counts[p.index()], 0..=10).suffix(" players"));
            ui.label(RichText::new(p.description()));
        }
    }

    fn plot_ui(&mut self, ui: &mut Ui) {
        let game_changed = match &self.last_game {
            None => true,
            Some((rating, counts, _)) => *rating != self.rating || *counts != self.counts
        };
        if game_changed {
            let players = player::play_game(player::all_players(), ROUNDS, &self.rating, &self.counts);
            self.last_game = Some((self.rating, self.counts.clone(), players));
        }
        let players = &self.last_game.as_ref().unwrap().2;
        if players.is_empty() {
            ui.centered_and_justified(|ui| ui.label("No players selected"));
            return;
        }

        let top_score: Vec<i32> = (0..ROUNDS).map(|i| {
            players.iter().map(|p| p.score()[i]).max().unwrap()
        }).collect();

        let points: Vec<Vec<[f64; 2]>> = players.iter().map(|p| {
            p.score().iter().enumerate().map(|(i, &score)| {
                let percent = if top_score[i] == 0 { 100.0 } else { score as f64 / top_score[i] as f64 * 100.0 };
                [i as f64, percent]
            }).collect()
        }).collect();

        let y_min = points.iter().flatten().map(|p| p[1]).fold(99.0, f64::min);
        let y_margin = (100.0 - y_min) * 0.05;
        let default_view = PlotBounds::from_min_max([0.0, y_min - y_margin], [(ROUNDS - 1) as f64, 100.0 + y_margin]);

        let lines: Vec<_> = players.iter().zip(points).map(|(p, points)| {
            Line::new(p.name(), PlotPoints::from(points)).color(gen_color(p.index())).id(Id::new(p.index()))
        }).collect();

        let plot = Plot::new("Plot")
            .x_axis_label("Round")
            .y_axis_label("Score")
            .y_axis_formatter(|_, _| String::new())
            .default_x_bounds(0.0, (ROUNDS - 1) as f64)
            .default_y_bounds(default_view.min()[1], default_view.max()[1])
            .allow_drag(true)
            .allow_zoom(true)
            .allow_scroll(false)
            .allow_boxed_zoom(true)
            .allow_double_click_reset(true);

        plot.show(ui, |ui| {
            if game_changed {
                ui.set_plot_bounds(default_view);
            }
            if ui.response().contains_pointer() {
                let scroll = ui.ctx().input(|i| i.smooth_scroll_delta.y);
                if scroll != 0.0 {
                    ui.zoom_bounds_around_hovered(Vec2::splat((scroll * 0.005).exp()));
                }
            }
            for line in lines {
                ui.line(line);
            }
        });
    }
}

impl eframe::App for DilemmaApp {
    fn ui(&mut self, ui: &mut Ui, _frame: &mut eframe::Frame) {
        Panel::left("Options")
            .resizable(true)
            .default_size(260.0)
            .min_size(200.0)
            .show_inside(ui, |ui| {
                ScrollArea::vertical().show(ui, |ui| {
                    ui.add_space(8.0);
                    self.options_ui(ui);
                    ui.add_space(8.0);
                });
            });

        CentralPanel::default().show_inside(ui, |ui| self.plot_ui(ui));
    }
}
