//
// Canadian Hydrological Model - The Canadian Hydrological Model (CHM) is a novel
// modular unstructured mesh based approach for hydrological modelling
// Copyright (C) 2018 Christopher Marsh
//
// This file is part of Canadian Hydrological Model.
//
// Canadian Hydrological Model is free software: you can redistribute it and/or
// modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Canadian Hydrological Model is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Canadian Hydrological Model.  If not, see
// <http://www.gnu.org/licenses/>.
//

#include "Sicart_ilwr.hpp"
REGISTER_MODULE_CPP(Sicart_ilwr);

Sicart_ilwr::Sicart_ilwr(config_file cfg)
        :module_base(parallel::data)
{

    depends("t");
    depends("rh");
    depends("iswr");
    depends("atm_trans");
    depends("cloud_frac");
    provides("ilwr");

    LOG_DEBUG << "Successfully instantiated module " << this->ID;
}

void Sicart_ilwr::init(mesh domain)
{

}
void Sicart_ilwr::run(mesh_elem& face)
{
    double T = face->face_data("t")+273.15; //C->K
    double tau = face->face_data("atm_trans");
    if( face->face_data("iswr") < 3.)
    {
        tau = face->face_data("cloud_frac");
    }


    double RH = face->face_data("rh") / 100.0;
    double es = mio::Atmosphere::vaporSaturationPressure(T);//mio::Atmosphere::saturatedVapourPressure(T);
    double e =  es * RH;
    e = e * 0.01; // pa->mb
    double sigma = 5.67*pow(10.0,-8.0); //boltzman

    double Lin = 1.24*pow(e/T,1.0/7.0)*(1.0+0.44*RH-0.18*tau)*sigma*pow(T,4.0);

    double svf = 1.; //default open view
    if (face->has_parameter("svf") && !is_nan(face->get_parameter("svf")))
    {
        svf = face->get_parameter("svf");
    }
    face->set_face_data("ilwr", svf*Lin);
}

Sicart_ilwr::~Sicart_ilwr()
{

}
