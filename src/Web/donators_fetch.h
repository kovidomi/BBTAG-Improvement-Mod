#pragma once
#include <vector>

const std::vector<std::string>& GetDonatorNames();
const std::vector<int>& GetDonatorTiers();
void StartAsyncDonatorsFetch();
