pub trait Strategy : std::fmt::Debug {
    fn play(&self, answers: &[bool]) -> bool;
}

fn betrayals(answers: &[bool]) -> i32 {
    answers.iter().filter(|a| !**a).count() as i32
}

#[derive(Debug, Clone, Copy)]
struct Clever;
impl Strategy for Clever {
    fn play(&self, answers: &[bool]) -> bool {
        if answers.len() == 0 { false }
        else if answers.len() <= 3 { true }
        else if answers.len() % 2 == 0 && answers[1] { false }
        else if answers[0] && !answers[1] && answers[2] && !answers[3] { false }
        else { answers[answers.len() - 1] }
    }
}

#[derive(Debug, Clone, Copy)]
struct Tolerant;
impl Strategy for Tolerant {
    fn play(&self, answers: &[bool]) -> bool {
        let mut tolerate = true;
        for i in 1..answers.len() {
            tolerate &= answers[i - 1] || answers[i];
        }
        tolerate
    }
}

#[derive(Debug, Clone, Copy)]
struct Agreeable;
impl Strategy for Agreeable {
    fn play(&self, _answers: &[bool]) -> bool {
        true
    }
}

#[derive(Debug, Clone, Copy)]
struct Averager;
impl Strategy for Averager {
    fn play(&self, answers: &[bool]) -> bool {
        betrayals(answers) <= answers.len() as i32 / 2
    }
}

#[derive(Debug, Clone, Copy)]
struct _Randomiser;
impl Strategy for _Randomiser {
    fn play(&self, _answers: &[bool]) -> bool {
        macroquad::rand::rand() % 2 == 0
    }
}

#[derive(Debug, Clone, Copy)]
struct Crook;
impl Strategy for Crook {
    fn play(&self, answers: &[bool]) -> bool {
        answers.len() == 0 || (answers.len() % 2 == 0 && answers[answers.len() - 1])
    }
}

#[derive(Debug, Clone, Copy)]
struct EvilAvg;
impl Strategy for EvilAvg {
    fn play(&self, answers: &[bool]) -> bool {
        answers.len() >= 2 && Averager.play(answers)
    }
}

#[derive(Debug, Clone, Copy)]
struct EvilNegative;
impl Strategy for EvilNegative {
    fn play(&self, answers: &[bool]) -> bool {
        answers.len() != 0 && !answers[answers.len() - 1]
    }
}

#[derive(Debug, Clone, Copy)]
struct EvilTf2T;
impl Strategy for EvilTf2T {
    fn play(&self, answers: &[bool]) -> bool {
        answers.len() >= 2 && (answers[answers.len() - 1] || answers[answers.len() - 2])
    }
}

#[derive(Debug, Clone, Copy)]
struct EvilTf1T;
impl Strategy for EvilTf1T {
    fn play(&self, answers: &[bool]) -> bool {
        answers.len() != 0 && answers[answers.len() - 1]
    }
}

#[derive(Debug, Clone, Copy)]
struct FlipFlop;
impl Strategy for FlipFlop {
    fn play(&self, answers: &[bool]) -> bool {
        answers.len() % 2 == 0
    }
}

#[derive(Debug, Clone, Copy)]
struct Forgive0;
impl Strategy for Forgive0 {
    fn play(&self, answers: &[bool]) -> bool {
        betrayals(answers) == 0
    }
}

#[derive(Debug, Clone, Copy)]
struct Forgive1;
impl Strategy for Forgive1 {
    fn play(&self, answers: &[bool]) -> bool {
        betrayals(answers) <= 1
    }
}

#[derive(Debug, Clone, Copy)]
struct Forgive2;
impl Strategy for Forgive2 {
    fn play(&self, answers: &[bool]) -> bool {
        betrayals(answers) <= 2
    }
}

#[derive(Debug, Clone, Copy)]
struct Historic;
impl Strategy for Historic {
    fn play(&self, answers: &[bool]) -> bool {
        answers.len() < 2 || answers[1]
    }
}

#[derive(Debug, Clone, Copy)]
struct Negative;
impl Strategy for Negative {
    fn play(&self, answers: &[bool]) -> bool {
        answers.len() == 0 || !answers[answers.len() - 1]
    }
}

#[derive(Debug, Clone, Copy)]
struct NiceAvg;
impl Strategy for NiceAvg {
    fn play(&self, answers: &[bool]) -> bool {
        answers.len() < 2 || Averager.play(answers)
    }
}

#[derive(Debug, Clone, Copy)]
struct Evil;
impl Strategy for Evil {
    fn play(&self, _answers: &[bool]) -> bool {
        false
    }
}

#[derive(Debug, Clone, Copy)]
struct Provocateur;
impl Strategy for Provocateur {
    fn play(&self, answers: &[bool]) -> bool {
        answers.len() != 0 && (answers.len() == 1 || answers[answers.len() - 1])
    }
}

#[derive(Debug, Clone, Copy)]
struct Tf1t;
impl Strategy for Tf1t {
    fn play(&self, answers: &[bool]) -> bool {
        answers.len() == 0 || answers[answers.len() - 1]
    }
}

#[derive(Debug, Clone, Copy)]
struct Tf2t;
impl Strategy for Tf2t {
    fn play(&self, answers: &[bool]) -> bool {
        answers.len() < 2 || answers[answers.len() - 1] || answers[answers.len() - 2]
    }
}

#[derive(Debug, Clone, Copy)]
struct Touchy;
impl Strategy for Touchy {
    fn play(&self, answers: &[bool]) -> bool {
        Tf1t.play(answers) && Tolerant.play(answers)
    }
}

pub fn all_strategies() -> Vec<Box<dyn Strategy>> {
    vec![
        Box::new(Clever),
        Box::new(Tolerant),
        Box::new(Agreeable),
        Box::new(Averager),
        // Box::new(_Randomiser),
        Box::new(Crook),
        Box::new(EvilAvg),
        Box::new(EvilNegative),
        Box::new(EvilTf2T),
        Box::new(EvilTf1T),
        Box::new(FlipFlop),
        Box::new(Forgive0),
        Box::new(Forgive1),
        Box::new(Forgive2),
        Box::new(Historic),
        Box::new(Negative),
        Box::new(NiceAvg),
        Box::new(Evil),
        Box::new(Provocateur),
        Box::new(Tf1t),
        Box::new(Tf2t),
        Box::new(Touchy)
    ]
}
