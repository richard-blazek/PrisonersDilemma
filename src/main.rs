use std::f32::consts::PI;

use macroquad::prelude::*;

mod player;
mod strategy;

const FONT_SIZE: u16 = 20;

fn is_cursor_in(rect: Rect) -> bool {
    let (x, y) = mouse_position();
    rect.x <= x && x < rect.x + rect.w && rect.y <= y && y < rect.y + rect.h
}

fn draw_string(s: &str, dest: Rect, color: Color) {
    let TextDimensions {width, height, ..} = measure_text(s, None, FONT_SIZE, 1.0);
    draw_text(s, dest.x + (dest.w - width) / 2.0, dest.y + (dest.h + height) / 2.0, FONT_SIZE as f32, color);
}

fn bounding_rect(players: &[player::Player], count: usize, i: usize, window_width: f32, window_height: f32) -> Rect {
    let height = players[i].score as f32 * window_height / players[0].score as f32;
    Rect::new(
        i as f32 * window_width / count as f32,
        window_height - height,
        window_width / count as f32 * 10.0 / 11.0,
        height,
    )
}

fn window_defaults() -> Conf {
    let mut conf = Conf::default();
    conf.window_title = "Prisonner's Dilemma".to_string();
    conf.window_width = 800;
    conf.window_height = 600;
    conf.window_resizable = true;
    conf
}

fn mouse_scroll() -> i32 {
    let scroll = mouse_wheel().1;
    if scroll > 0.0 { 1 } else if scroll < 0.0 { -1 } else { 0 }
}


#[macroquad::main(window_defaults)]
async fn main() {
    let mut rounds = 5;

    loop {
        let players = player::play_game(player::all_players(), rounds);

        let (window_width, window_height) = (screen_width(), screen_height());

        let round_rect = Rect::new(window_width / 2.0, 0.0, window_width / 2.0, 24.0);
        draw_string(&format!("Prisonner's dilemma: {} rounds", rounds + 1), round_rect, WHITE);

        for (i, player) in players.iter().enumerate() {
            let area = bounding_rect(&players, players.len(), i, window_width, window_height);
            let color = Color::new(i as f32 / players.len() as f32, 1.0 - i as f32 / players.len() as f32, 0.0, 1.0);
            draw_rectangle(area.x, area.y, area.w, area.h, color);

            if is_cursor_in(area) {
                draw_rectangle_lines(area.x, area.y, area.w, area.h, 2.0, BLUE);
            }

            let score_rect = Rect::new(area.x, area.y + 5.0, area.w, 24.0);
            draw_string(&format!("{}", player.score * 1000 / players[0].score), score_rect, BLACK);

            draw_text_ex(player.name().as_str(), area.x + area.w / 3.0, area.y + 35.0, TextParams {
                font: None, font_size: FONT_SIZE, font_scale: 1.0, font_scale_aspect: 1.0, rotation: PI / 2.0, color: BLACK
            });
        }

        rounds = i32::clamp(rounds as i32 + mouse_scroll(), 1, 99) as usize;
        next_frame().await
    }
}
