#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include "CSV.h"

CSVFrame::CSVFrame(std::map<char*, std::vector<double>> const& data) {
	internalFrame = data;
	for (auto const& kv : internalFrame)
	{
		colLens.insert(colLens.end(), kv.second.size());
		/*std::cout << kv.first << ": [ ";
		for (auto const& data : kv.second)
			std::cout << data << ", ";
		std::cout << "\b\b ]\n";*/
	}
}

CSVFrame::~CSVFrame() {}

void CSVFrame::AddColumn(std::map<char*, std::vector<double>> const& col) {
	
	std::map<char*, std::vector<double>> localCol = col;
	localCol.merge(internalFrame);
	internalFrame = localCol;
	for (auto const& kv : col)
	{
		colLens.insert(colLens.end(), kv.second.size());
	}
	/*for (auto const& kv : internalFrame)
	{
		std::cout << kv.first << ": [ ";
		for (auto const& data : kv.second)
			std::cout << data << " ";
		std::cout << "]\n";
	}*/
}

void CSVFrame::WriteToCSV(char* fileName) {
	if (internalFrame.empty()) {
		std::cout << "Could not write to file " << std::filesystem::absolute(fileName) << ". CSVFrame is empty.\n";
		return;
	}
	int colLensMaxVal = *std::max_element(colLens.begin(), colLens.end());
	std::ofstream fileObj(fileName);
	if (fileObj.is_open())
	{
		while (fileObj.good())
		{
			for (auto const& kv : internalFrame)
			{
				fileObj << kv.first << ',';
			}
			fileObj << '\n';

			for (int i = 0; i < colLensMaxVal; i++)
			{
				int j = 0;
				for (auto const& kv : internalFrame)
				{
					if (i >= colLens[j]) {
						fileObj << ',';
						j++;
						continue;
					}
					fileObj << kv.second[i] << ',';
					j++;
				}
				fileObj << '\n';
			}
			break;
		}
		fileObj.close();
		std::cout << "A file has been written at " << std::filesystem::absolute(fileName) << '\n';
	}
}