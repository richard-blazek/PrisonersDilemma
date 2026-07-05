use std::collections::hash_map::DefaultHasher;
use std::hash::{Hash, Hasher};

pub trait Strategy : std::fmt::Debug {
    fn play(&self, answers: &[bool]) -> bool;
    fn description(&self) -> String;
}

fn betrayals(answers: &[bool]) -> i32 {
    answers.iter().filter(|a| !**a).count() as i32
}

fn randomize<T: Strategy>(strategy: &T, answers: &[bool]) -> u64 {
    let name = format!("{:?}", strategy);
    let mut h = DefaultHasher::new();
    (name, answers).hash(&mut h);
    h.finish()
}

#[derive(Debug, Clone, Copy)]
struct Good;
impl Strategy for Good {
    fn play(&self, _answers: &[bool]) -> bool {
        true
    }

    fn description(&self) -> String {
        "Always cooperates".to_string()
    }
}

#[derive(Debug, Clone, Copy)]
struct Evil;
impl Strategy for Evil {
    fn play(&self, _answers: &[bool]) -> bool {
        false
    }

    fn description(&self) -> String {
        "Always betrays".to_string()
    }
}

#[derive(Debug, Clone, Copy)]
struct TitForTat;
impl Strategy for TitForTat {
    fn play(&self, answers: &[bool]) -> bool {
        answers.len() == 0 || answers[answers.len() - 1]
    }

    fn description(&self) -> String {
        "Copies opponent's last move (cooperating in the first round)".to_string()
    }
}

#[derive(Debug, Clone, Copy)]
struct Forgive0;
impl Strategy for Forgive0 {
    fn play(&self, answers: &[bool]) -> bool {
        betrayals(answers) == 0
    }

    fn description(&self) -> String {
        "Cooperates until gets betrayed, then always betrays".to_string()
    }
}

#[derive(Debug, Clone, Copy)]
struct TitForTwoTats;
impl Strategy for TitForTwoTats {
    fn play(&self, answers: &[bool]) -> bool {
        answers.len() < 2 || answers[answers.len() - 1] || answers[answers.len() - 2]
    }

    fn description(&self) -> String {
        "Cooperates, unless the opponent betrayed in both of the last two rounds".to_string()
    }
}

#[derive(Debug, Clone, Copy)]
struct TwoTitsForTat;
impl Strategy for TwoTitsForTat {
    fn play(&self, answers: &[bool]) -> bool {
        answers.iter().rev().take(2).all(|a| *a)
    }

    fn description(&self) -> String {
        "Cooperates, unless opponent betrayed in either of the last two rounds".to_string()
    }
}

#[derive(Debug, Clone, Copy)]
struct Pavlov;
impl Strategy for Pavlov {
    fn play(&self, answers: &[bool]) -> bool {
        answers.iter().fold(true, |me, &a| me == a)
    }

    fn description(&self) -> String {
        "Starts by cooperating, switches its move every time opponent betrays".to_string()
    }
}

#[derive(Debug, Clone, Copy)]
struct Random;
impl Strategy for Random {
    fn play(&self, answers: &[bool]) -> bool {
        randomize(self, answers) % 2 == 0
    }

    fn description(&self) -> String {
        "Cooperates or betrays randomly".to_string()
    }
}

#[derive(Debug, Clone, Copy)]
struct Joss;
impl Strategy for Joss {
    fn play(&self, answers: &[bool]) -> bool {
        (answers.len() == 0 || answers[answers.len() - 1]) && randomize(self, answers) % 10 != 0
    }

    fn description(&self) -> String {
        "Copies opponent's last move (cooperating in the first round) with 90% chance and betrays with 10% chance".to_string()
    }
}

#[derive(Debug, Clone, Copy)]
struct Davis;
impl Strategy for Davis {
    fn play(&self, answers: &[bool]) -> bool {
        answers.len() < 10 || betrayals(answers) == 0
    }

    fn description(&self) -> String {
        "Cooperates for the first 10 rounds, then betrays forever if ever betrayed".to_string()
    }
}

#[derive(Debug, Clone, Copy)]
struct Shubik;
impl Strategy for Shubik {
    fn play(&self, answers: &[bool]) -> bool {
        let (mut k, mut r) = (0, 0);
        for a in answers {
            if r > 0 {
                r -= 1;
            } else if !*a {
                k += 1;
                r = k;
            }
        }
        r == 0
    }

    fn description(&self) -> String {
        "Punishes each betrayal with a run of betrayals that grows one round longer each time".to_string()
    }
}

#[derive(Debug, Clone, Copy)]
struct SteinRapoport;
impl Strategy for SteinRapoport {
    fn play(&self, answers: &[bool]) -> bool {
        answers.len() < 4 || answers[answers.len() - 1]
    }

    fn description(&self) -> String {
        "Cooperates for the first 4 rounds, then copies opponent's last move".to_string()
    }
}

#[derive(Debug, Clone, Copy)]
struct Tullock;
impl Strategy for Tullock {
    fn play(&self, answers: &[bool]) -> bool {
        answers.len() < 11 || {
            let rate = answers.iter().rev().take(10).filter(|a| **a).count() as f64 / 10.0 - 0.1;
            (randomize(self, answers) % 1000) as f64 / 1000.0 < rate
        }
    }

    fn description(&self) -> String {
        "Cooperates for the first 11 rounds, then cooperates 10% less often than opponent did in the last 10 rounds".to_string()
    }
}

#[derive(Debug, Clone, Copy)]
struct Feld;
impl Strategy for Feld {
    fn play(&self, answers: &[bool]) -> bool {
        answers.last().map_or(true, |a| *a)
            && (randomize(self, answers) % 1000) as f64 / 1000.0 < 1.0 - (answers.len() as f64 / 200.0).min(0.5)
    }

    fn description(&self) -> String {
        "Copies opponent's last move (cooperating in the first round), except its chance of cooperating declines from 100% to 50% over 100 rounds".to_string()
    }
}

pub fn all_strategies() -> Vec<Box<dyn Strategy>> {
    vec![
        Box::new(Good),
        Box::new(Evil),
        Box::new(TitForTat),
        Box::new(Forgive0),
        Box::new(TitForTwoTats),
        Box::new(TwoTitsForTat),
        Box::new(Pavlov),
        Box::new(Random),
        Box::new(Joss),
        Box::new(Davis),
        Box::new(Shubik),
        Box::new(SteinRapoport),
        Box::new(Tullock),
        Box::new(Feld),
    ]
}
