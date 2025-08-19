use crate::strategy;

pub struct Player {
    pub strategy: Box<dyn strategy::Strategy>,
    pub score: i32,
}

impl Player {
    pub fn name(&self) -> String {
        format!("{:?}", self.strategy)
    }
}

pub fn all_players() -> Vec<Player> {
    strategy::all_strategies().into_iter().map(|strategy| {
        Player { strategy, score: 0 }
    }).collect()
}

const RATING: [[i32; 2]; 2] = [[1, 5], [0, 3]];

impl PartialEq for Player {
    fn eq(&self, other: &Self) -> bool {
        self.score == other.score
    }
}

impl Eq for Player {}

impl PartialOrd for Player {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(other.score.cmp(&self.score))
    }
}

impl Ord for Player {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        other.score.cmp(&self.score)
    }
}

pub fn play_game(mut players: Vec<Player>, loops: usize) -> Vec<Player> {
    let mut answers1 = vec![false; loops];
    let mut answers2 = vec![false; loops];

    for i in 0..players.len() {
        for j in 0..players.len() {
            for k in 0..loops {
                answers1[k] = players[i].strategy.play(&answers2[0..k]);
                answers2[k] = players[j].strategy.play(&answers1[0..k]);
                players[i].score += RATING[answers1[k] as usize][answers2[k] as usize];
            }
        }
    }

    players.sort();
    players
}
