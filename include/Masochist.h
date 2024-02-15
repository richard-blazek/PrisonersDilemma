#pragma once

Player masochist([](const vector<bool>& answers)
{
	return answers.empty()||!answers.back();
}, "Masochist");