#pragma once
#include <stdint.h>
#include <utility>

#include "AnalyseType.h"
#include "../SoilMath/SoilMath.h"

namespace SoilAnalyzer
{
	class AnalysisResults
	{
	public:
		AnalysisResults();
		~AnalysisResults();
		void Analyse(AnalyseType analyseType);
		uint8_t version;
		ucharStat_t ClassRoundness;
		std::pair<float, float> LAB;
		ucharStat_t PSD;
		ucharStat_t RI;
	};
}
