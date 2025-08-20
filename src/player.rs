use crate::strategy;

pub struct Player {
    strategy: Box<dyn strategy::Strategy>,
    points: i32,
}

impl Player {
    pub fn name(&self) -> String {
        format!("{:?}", self.strategy)
    }

    pub fn score(&self) -> i32 {
        self.points
    }
}

pub fn all_players() -> Vec<Player> {
    strategy::all_strategies().into_iter().map(|strategy| {
        Player { strategy, points: 0 }
    }).collect()
}

pub fn play_game(mut players: Vec<Player>, rounds: usize, rating: &[[i32; 2]]) -> Vec<Player> {
    let mut answers1 = vec![false; rounds];
    let mut answers2 = vec![false; rounds];

    for i in 0..players.len() {
        for j in 0..players.len() {
            for k in 0..rounds {
                answers1[k] = players[i].strategy.play(&answers2[0..k]);
                answers2[k] = players[j].strategy.play(&answers1[0..k]);
                players[i].points += rating[answers1[k] as usize][answers2[k] as usize];
            }
        }
    }

    players.sort_by_key(|p| -p.points);
    players
}
