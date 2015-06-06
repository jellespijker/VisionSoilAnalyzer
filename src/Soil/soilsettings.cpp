#include "soilsettings.h"

namespace SoilAnalyzer {
    SoilSettings::SoilSettings()
    {

    }

    void SoilSettings::LoadSettings(string filename)
    {
        std::ifstream ifs(filename.c_str());
        boost::archive::xml_iarchive ia(ifs);
        ia >> boost::serialization::make_nvp("SoilSettings", *this);
    }

    void SoilSettings::SaveSettings(string filename)
    {
        std::ofstream ofs(filename.c_str());
        boost::archive::xml_oarchive oa(ofs);
        oa << boost::serialization::make_nvp("SoilSettings", *this);
    }
}
