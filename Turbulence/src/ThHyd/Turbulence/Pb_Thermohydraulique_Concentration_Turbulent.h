/****************************************************************************
* Copyright (c) 2015 - 2016, CEA
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//
// File:        Pb_Thermohydraulique_Concentration_Turbulent.h
// Directory:   $TRUST_ROOT/src/ThHyd/Turbulence
// Version:     /main/13
//
//////////////////////////////////////////////////////////////////////////////


#ifndef Pb_Thermohydraulique_Concentration_Turbulent_included
#define Pb_Thermohydraulique_Concentration_Turbulent_included

#include <Pb_Fluide_base.h>
#include <Navier_Stokes_Turbulent.h>
#include <Convection_Diffusion_Temperature_Turbulent.h>
#include <Convection_Diffusion_Concentration_Turbulent.h>

class Champ_Fonc;


//////////////////////////////////////////////////////////////////////////////
//
// .DESCRIPTION
//    Classe Pb_Thermohydraulique_Concentration_Turbulent
//    Cette classe represente un probleme de thermohydraulique
//    avec modelisation de la turbulence:
//     - Equations de Navier_Stokes en regime turbulent
//       pour un fluide incompressible
//     - Equation d'energie en regime turbulent
//     - Equation de Convection-Diffusion d'une ou plusieurs concentrations
//       en regime turbulent
// .SECTION voir aussi
//     Pb_Fluide_base
//////////////////////////////////////////////////////////////////////////////
class Pb_Thermohydraulique_Concentration_Turbulent : public Pb_Fluide_base
{

  Declare_instanciable(Pb_Thermohydraulique_Concentration_Turbulent);

public:

  int nombre_d_equations() const;
  const Equation_base& equation(int) const ;
  Equation_base& equation(int);
  inline const Champ_Fonc& viscosite_turbulente() const;
  void associer_milieu_base(const Milieu_base& );
  int verifier();

protected:

  Navier_Stokes_Turbulent eq_hydraulique;
  Convection_Diffusion_Temperature_Turbulent eq_thermique;
  Convection_Diffusion_Concentration_Turbulent eq_concentration;

};


// Description:
//    Renvoie le champ representant la viscosite turbulente.
// Precondition:
// Parametre:
//    Signification:
//    Valeurs par defaut:
//    Contraintes:
//    Acces:
// Retour: Champ_Fonc&
//    Signification: le champ representant la viscosite turbulente
//    Contraintes: reference constante
// Exception:
// Effets de bord:
// Postcondition: la methode ne modifie pas l'objet
inline const Champ_Fonc& Pb_Thermohydraulique_Concentration_Turbulent::viscosite_turbulente() const
{
  return eq_hydraulique.viscosite_turbulente();
}

#endif
