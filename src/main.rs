use std::f32::consts::PI;

use macroquad::color::{Color, BLACK};
use macroquad::math::Rect;
use macroquad::shapes::draw_rectangle;
use macroquad::text::{draw_text, draw_text_ex, measure_text, TextDimensions, TextParams};
use macroquad::window::{next_frame, screen_height, screen_width};
use egui_macroquad::egui;

mod player;
mod strategy;

const FONT_SIZE: u16 = 20;

fn draw_string(s: &str, dest: Rect, color: Color) {
    let TextDimensions {width, height, ..} = measure_text(s, None, FONT_SIZE, 1.0);
    draw_text(s, dest.x + (dest.w - width) / 2.0, dest.y + (dest.h + height) / 2.0, FONT_SIZE as f32, color);
}

fn bounding_rect(players: &[player::Player], i: usize, canvas: Rect) -> Rect {
    let h = players[i].score() as f32 * canvas.h / players[0].score() as f32;
    let w = canvas.w / players.len() as f32;
    Rect::new(canvas.x + w * i as f32, canvas.y + canvas.h - h, w * 0.91, h)
}

#[macroquad::main("Prisonner's dilemma")]
async fn main() {
    let mut rating = [[1, 5], [0, 3]];
    let mut rounds = 5;

    loop {
        let all_players = player::all_players();
        let players = player::play_game(all_players, rounds, &rating);
        
        let mut offset_y = 0.0;
        egui_macroquad::ui(|egui_ctx| {
            let panel = egui::TopBottomPanel::top("Options")
                .show(egui_ctx, |ui| {
                    ui.horizontal(|ui| {
                        ui.add(egui::Slider::new(&mut rounds, 2..=100).text("rounds"));
                        ui.vertical(|ui| {
                            ui.add(egui::Slider::new(&mut rating[1][1], 0..=9).text("cooperation"));
                            ui.add(egui::Slider::new(&mut rating[0][1], 0..=9).text("temptation"));
                        });
                        ui.vertical(|ui| {
                            ui.add(egui::Slider::new(&mut rating[1][0], 0..=9).text("sucker"));
                            ui.add(egui::Slider::new(&mut rating[0][0], 0..=9).text("conflict"));
                        });
                    });
                });
            offset_y = panel.response.rect.height();
        });
        egui_macroquad::draw();

        let canvas = Rect::new(0.0, offset_y, screen_width(), screen_height() - offset_y);

        for (i, player) in players.iter().enumerate() {
            let area = bounding_rect(&players, i, canvas);
            let color = Color::new(i as f32 / players.len() as f32, 1.0 - i as f32 / players.len() as f32, 0.0, 1.0);
            draw_rectangle(area.x, area.y, area.w, area.h, color);

            let score_rect = Rect::new(area.x, area.y + 5.0, area.w, 24.0);
            draw_string(player.score().to_string().as_str(), score_rect, BLACK);

            draw_text_ex(player.name().as_str(), area.x + area.w / 3.0, area.y + 35.0, TextParams {
                font: None, font_size: FONT_SIZE, font_scale: 1.0, font_scale_aspect: 1.0, rotation: PI / 2.0, color: BLACK
            });
        }

        next_frame().await
    }
}
