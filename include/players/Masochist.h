#pragma once

Player masochist([](const std::vector<bool>& answers)
{
	return answers.empty()||!answers.back();
}, "Masochist");