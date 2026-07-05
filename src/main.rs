use eframe::egui::{Align2, CentralPanel, Color32, Id, Panel, Slider, Ui, Window, ecolor::Hsva};
use egui_plot::{Plot, Line, PlotPoints};

mod player;
mod strategy;

#[cfg(not(target_arch = "wasm32"))]
fn main() -> eframe::Result {
    eframe::run_native(
        "Prisoner's dilemma",
        Default::default(),
        Box::new(|_| Ok(Box::<DilemmaApp>::default())),
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
                Box::new(|_| Ok(Box::<DilemmaApp>::default())),
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

struct DilemmaApp {
    rating: [[i32; 2]; 2],
    selection: Option<usize>,
}

impl Default for DilemmaApp {
    fn default() -> Self {
        Self { rating: [[1, 5], [0, 3]], selection: None }
    }
}

fn gen_color(index: usize) -> Color32 {
    const PHI: f32 = 1.618_033_988_749_894;
    const SV: [(f32, f32); 3] = [(1.0, 1.0), (1.0, 0.5), (0.5, 1.0)];

    let h = (index as f32 / 6.0 / PHI).fract(); 
    let (s, v) = SV[index % SV.len()];
    Color32::from(Hsva::new(h, s, v, 1.0))
}

impl eframe::App for DilemmaApp {
    fn ui(&mut self, ui: &mut Ui, _frame: &mut eframe::Frame) {
        const ROUNDS: usize = 100;
        let all_players = player::all_players();
        let players = player::play_game(all_players, ROUNDS, &self.rating);

        Panel::top("Options").show_inside(ui, |ui| {
            ui.horizontal(|ui| {
                ui.add(Slider::new(&mut self.rating[1][1], 0..=10).text("cooperation"));
                ui.add(Slider::new(&mut self.rating[0][1], 0..=10).text("temptation"));
            });
            ui.horizontal(|ui| {
                ui.add(Slider::new(&mut self.rating[1][0], 0..=10).text("sucker"));
                ui.add(Slider::new(&mut self.rating[0][0], 0..=10).text("conflict"));
            });
        });

        CentralPanel::default().show_inside(ui, |ui| {
            let plot = Plot::new("Plot")
                .allow_drag(false)
                .allow_zoom(false)
                .allow_scroll(false)
                .allow_boxed_zoom(false)
                .allow_double_click_reset(false);

            let top_score: Vec<i32> = (0..ROUNDS).map(|i| {
                players.iter().map(|p| p.score()[i]).max().unwrap()
            }).collect();

            let lines: Vec<_> = players.iter().map(|p| {
                let points: PlotPoints = p.score().iter().enumerate().map(|(i, &score)| {
                    [i as f64, score as f64 / top_score[i] as f64 * 100.0]
                }).collect();
                Line::new(p.name(), points).color(gen_color(p.index())).id(Id::new(p.index()))
            }).collect();

            let shown = plot.show(ui, |ui| {
                for line in lines {
                    ui.line(line);
                }
            });

            if shown.response.clicked() {
                self.selection = None;
                for p in &players {
                    if shown.hovered_plot_item == Some(Id::new(p.index())) {
                        self.selection = Some(p.index())
                    }
                }
            }

            if let Some(selection) = self.selection {
                let player = players.iter().find(|p| p.index() == selection).unwrap();
                let mut open = true;
                Window::new(player.name())
                    .collapsible(false)
                    .pivot(Align2::CENTER_CENTER)
                    .open(&mut open)
                    .show(ui.ctx(), |ui| ui.label(player.description()));
                if !open {
                    self.selection = None;
                }
            }
        });
    }
}
