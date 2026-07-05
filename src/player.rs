use crate::strategy;

pub struct Player {
    strategy: Box<dyn strategy::Strategy>,
    points: Vec<i32>,
    i: usize
}

impl Player {
    pub fn name(&self) -> String {
        format!("{:?}", self.strategy)
    }

    pub fn description(&self) -> String {
        self.strategy.description()
    }

    pub fn score<'a>(&'a self) -> &'a [i32] {
        &self.points
    }

    pub fn index(&self) -> usize {
        self.i
    }
}

pub fn all_players() -> Vec<Player> {
    strategy::all_strategies().into_iter().enumerate().map(|(i, strategy)| {
        Player { strategy, points: Vec::new(), i }
    }).collect()
}

pub fn play_game(mut players: Vec<Player>, rounds: usize, rating: &[[i32; 2]]) -> Vec<Player> {
    let mut answers1 = vec![false; rounds];
    let mut answers2 = vec![false; rounds];

    for i in 0..players.len() {
        players[i].points = vec![0; rounds];

        for j in 0..players.len() {
            for k in 0..rounds {
                answers1[k] = players[i].strategy.play(&answers2[0..k]);
                answers2[k] = players[j].strategy.play(&answers1[0..k]);
                players[i].points[k] += rating[answers1[k] as usize][answers2[k] as usize];
            }
        }

        for k in 1..rounds {
            players[i].points[k] += players[i].points[k-1];
        }
    }

    players.sort_by_key(|p| -p.points[rounds-1]);
    players
}
