#pragma once

class CSVFrame {

	public:
		CSVFrame(std::map<char*, std::vector<double>> const& data);
		~CSVFrame();
		void AddColumn(std::map<char*, std::vector<double>> const& col);
		void WriteToCSV(char* fileName);

	private:
		class std::map<char*, std::vector<double>> internalFrame;
		class std::vector<int> colLens;
};
