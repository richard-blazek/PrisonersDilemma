use eframe::egui;
use egui_plot::{Plot, Bar, BarChart};

mod player;
mod strategy;

fn main() -> eframe::Result {
    eframe::run_native(
        "Prisonner's dilemma",
        Default::default(),
        Box::new(|_| Ok(Box::<DilemmaApp>::default())),
    )
}

struct DilemmaApp {
    rating: [[i32; 2]; 2],
    rounds: usize,
}

impl Default for DilemmaApp {
    fn default() -> Self {
        Self { rating: [[1, 5], [0, 3]], rounds: 10 }
    }
}

fn rgb(r: f32, g: f32, b: f32) -> egui::Color32 {
    let r = (r * 255.0).clamp(0.0, 255.0) as u8;
    let g = (g * 255.0).clamp(0.0, 255.0) as u8;
    let b = (b * 255.0).clamp(0.0, 255.0) as u8;
    egui::Color32::from_rgb(r, g, b)
}

fn mix_colour(score: i32, high: i32, low: i32) -> egui::Color32 {
    let rel = (score - low) as f32 / (high - low) as f32;
    if rel > 0.5 {
        rgb(1.0 - rel, 1.0, 0.0)
    } else {
        rgb(1.0 - rel, 2.0 * rel, 0.0)
    }
}

impl eframe::App for DilemmaApp {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        let all_players = player::all_players();
        let players = player::play_game(all_players, self.rounds, &self.rating);

        egui::TopBottomPanel::top("Options").show(ctx, |ui| {
            ui.add(egui::Slider::new(&mut self.rounds, 1..=100).text("rounds"));

            ui.horizontal(|ui| {
                ui.add(egui::Slider::new(&mut self.rating[1][1], 0..=10).text("cooperation"));
                ui.add(egui::Slider::new(&mut self.rating[0][1], 0..=10).text("temptation"));
            });
            ui.horizontal(|ui| {
                ui.add(egui::Slider::new(&mut self.rating[1][0], 0..=10).text("sucker"));
                ui.add(egui::Slider::new(&mut self.rating[0][0], 0..=10).text("conflict"));
            });
        });

        egui::CentralPanel::default().show(ctx, |ui| {
            let plot = Plot::new("Plot");

            let low = players[players.len()-1].score();
            let high = players[0].score();
            let bars : Vec<_> = players.iter().enumerate().map(|(i, p)| {
                Bar::new(i as f64, p.score() as f64)
                    .name(p.name())
                    .fill(mix_colour(p.score(), high, low))
            }).collect();

            plot.show(ui, |ui| ui.add(BarChart::new("Score", bars)));
        });
    }
}
