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
// File:        Transport_K_ou_Eps_base.h
// Directory:   $TRUST_ROOT/src/ThHyd/Turbulence
// Version:     /main/17
//
//////////////////////////////////////////////////////////////////////////////


#ifndef Transport_K_ou_Eps_base_included
#define Transport_K_ou_Eps_base_included

#include <Equation_base.h>
#include <Ref_Champ_Inc.h>
#include <Mod_turb_hyd_RANS_Bicephale.h>
#include <Ref_Milieu_base.h>
#include <Ref_Mod_turb_hyd_RANS_Bicephale.h>
#include <Ref_Champ_Inc_base.h>

//////////////////////////////////////////////////////////////////////////////
//
// .DESCRIPTION
//    Classe Transport_K_ou_Eps_base
//    Classe de base pour l'equation
//    de transport des modeles k_Epsilon dans une approche ou K et Epsilon sont traites par deux equations differentes.
// .SECTION voir aussi
//
//////////////////////////////////////////////////////////////////////////////


class Transport_K_ou_Eps_base: public Equation_base
{

  Declare_base_sans_constructeur(Transport_K_ou_Eps_base);

public:

  Transport_K_ou_Eps_base();
  void set_param(Param&);
  double calculer_pas_de_temps() const;
  inline void associer_vitesse(const Champ_base& );
  virtual void associer_milieu_base(const Milieu_base&);
  virtual void associer_modele_turbulence(const Mod_turb_hyd_RANS_Bicephale& )=0;
  virtual const Milieu_base& milieu() const ;
  virtual Milieu_base& milieu() ;
  void associer(const Equation_base&);
  void discretiser();
  virtual void discretiser_K_Eps(const Schema_Temps_base&, Zone_dis&, Champ_Inc&) const;

  virtual int controler_variable();
  void valider_iteration();
  virtual inline const Champ_Inc& inconnue() const;
  virtual inline Champ_Inc& inconnue();
  inline const Mod_turb_hyd_RANS_Bicephale& modele_turbulence() const;
  inline Mod_turb_hyd_RANS_Bicephale& modele_turbulence();

  const Champ_base& get_champ( const Motcle& nom ) const;
//  void creer_champ( const Motcle& motlu );

  inline void transporte_K();
  inline void transporte_Eps();
  inline bool transporte_t_il_K() const;

protected:

  Champ_Inc le_champ_;
  Champ_Fonc residu_;

  REF(Milieu_base) le_fluide;
  REF(Champ_Inc_base) la_vitesse_transportante;
  REF(Mod_turb_hyd_RANS_Bicephale) mon_modele;

  bool transporte_K_;
};
// Description:
//    Renvoie le champ inconnue de l'equation.
//    Un champ vecteur contenant K ou epsilon.
// Precondition:
// Parametre:
//    Signification:
//    Valeurs par defaut:
//    Contraintes:
//    Acces:
// Retour: Champ_Inc&
//    Signification: le champ inconnue de l'equation
//    Contraintes:
// Exception:
// Effets de bord:
// Postcondition:
inline Champ_Inc& Transport_K_ou_Eps_base::inconnue()
{
  return le_champ_;
}


// Description:
//    Renvoie le champ inconnue de l'equation.
//    Un champ vecteur contenant K ou epsilon.
//    (version const)
// Precondition:
// Parametre:
//    Signification:
//    Valeurs par defaut:
//    Contraintes:
//    Acces:
// Retour: Champ_Inc&
//    Signification: le champ inconnue de l'equation
//    Contraintes: reference constante
// Exception:
// Effets de bord:
// Postcondition: la methode ne modifie pas l'objet
inline const Champ_Inc& Transport_K_ou_Eps_base::inconnue() const
{
  return le_champ_;
}

// Description:
//   Renvoie le modele de turbulence associe a l'equation.
//   (version const)
// Precondition: un modele de turbulence doit avoir ete associe
// Parametre:
//    Signification:
//    Valeurs par defaut:
//    Contraintes:
//    Acces:
// Retour: Modele_turbulence_hyd_K_Eps&
//    Signification: le modele de turbulence associe a l'equation
//    Contraintes: reference constante
// Exception:
// Effets de bord:
// Postcondition: la methode ne modifie pas l'objet
inline const Mod_turb_hyd_RANS_Bicephale& Transport_K_ou_Eps_base::modele_turbulence() const
{
  assert(mon_modele.non_nul());
  return mon_modele.valeur();
}


// Description:
//   Renvoie le modele de turbulence associe a l'equation.
// Precondition: un modele de turbulence doit avoir ete associe
// Parametre:
//    Signification:
//    Valeurs par defaut:
//    Contraintes:
//    Acces:
// Retour: Modele_turbulence_hyd_K_Eps&
//    Signification: le modele de turbulence associe a l'equation
//    Contraintes:
// Exception:
// Effets de bord:
// Postcondition:
inline Mod_turb_hyd_RANS_Bicephale& Transport_K_ou_Eps_base::modele_turbulence()
{
  assert(mon_modele.non_nul());
  return mon_modele.valeur();
}

inline void Transport_K_ou_Eps_base::associer_vitesse(const Champ_base& vit)
{
  la_vitesse_transportante = ref_cast(Champ_Inc_base,vit);
}

inline void Transport_K_ou_Eps_base::transporte_K()
{
  transporte_K_ = true;
}

inline void Transport_K_ou_Eps_base::transporte_Eps()
{
  transporte_K_ = false;
}

inline bool Transport_K_ou_Eps_base::transporte_t_il_K() const
{
  return transporte_K_;
}

#endif
