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
// File:        Eval_Diff_K_Eps_QC_VDF_Elem.h
// Directory:   $TRUST_ROOT/src/VDF/Turbulence
// Version:     /main/11
//
//////////////////////////////////////////////////////////////////////////////

#ifndef Eval_Diff_K_Eps_QC_VDF_Elem_included
#define Eval_Diff_K_Eps_QC_VDF_Elem_included

#include <Eval_Diff_K_Eps_VDF.h>
#include <CL_Types_include.h>
#include <Champ_Fonc.h>
#include <Eval_VDF_Elem.h>
#include <Fluide_Quasi_Compressible.h>
#include <Zone_VDF.h>

class Eval_Diff_K_Eps_QC_VDF_Elem : public Eval_Diff_K_Eps_VDF, public Eval_VDF_Elem
{

public:
  inline Eval_Diff_K_Eps_QC_VDF_Elem();

  template <typename Type_Double> inline void flux_face(const DoubleTab&, const int , const Symetrie&, int, Type_Double& ) const;
  template <typename Type_Double> inline void flux_face(const DoubleTab&, const int , const Periodique&, int, Type_Double& ) const;
  template <typename Type_Double> inline void flux_face(const DoubleTab&, const int , const Neumann_sortie_libre&, int, Type_Double& ) const;
  template <typename Type_Double> inline void flux_face(const DoubleTab&, const int , const Dirichlet_entree_fluide&, int, Type_Double& ) const;
  template <typename Type_Double> inline void flux_face(const DoubleTab&, const int , const Dirichlet_paroi_fixe&, int, Type_Double& ) const;
  template <typename Type_Double> inline void flux_face(const DoubleTab&, const int , const Dirichlet_paroi_defilante&, int, Type_Double& ) const;
  template <typename Type_Double> inline void flux_face(const DoubleTab&, const int , const Neumann_paroi_adiabatique&, int, Type_Double& ) const;
  template <typename Type_Double> inline void flux_face(const DoubleTab&, const int , const Neumann_paroi&, int, Type_Double& ) const;
  template <typename Type_Double> inline void flux_face(const DoubleTab&, const int , const int, const int, const Echange_externe_impose&, const int, Type_Double& ) const;
  template <typename Type_Double> inline void flux_face(const DoubleTab&, const int , const Echange_global_impose&, int, Type_Double& ) const;
  template <typename Type_Double> inline void flux_faces_interne(const DoubleTab&, const int ,  Type_Double& ) const;

  template <typename Type_Double> inline void coeffs_face(const int, const int, const Symetrie&, Type_Double& , Type_Double&  ) const;
  template <typename Type_Double> inline void coeffs_face(const int, const int, const Neumann_sortie_libre&, Type_Double& , Type_Double&  ) const;
  template <typename Type_Double> inline void coeffs_face(const int, const int, const Dirichlet_entree_fluide&, Type_Double& , Type_Double&  ) const;
  template <typename Type_Double> inline void coeffs_face(const int, const int, const Dirichlet_paroi_fixe&, Type_Double& , Type_Double&  ) const;
  template <typename Type_Double> inline void coeffs_face(const int, const int, const Dirichlet_paroi_defilante&, Type_Double& , Type_Double&  ) const;
  template <typename Type_Double> inline void coeffs_face(const int, const int, const Neumann_paroi_adiabatique&, Type_Double& , Type_Double&  ) const;
  template <typename Type_Double> inline void coeffs_face(const int, const int, const Neumann_paroi&, Type_Double& , Type_Double&  ) const;
  template <typename Type_Double> inline void coeffs_face(const int, const int, const int, const int, const Echange_externe_impose&, Type_Double& , Type_Double&  ) const;
  template <typename Type_Double> inline void coeffs_face(const int, const int, const Echange_global_impose&, Type_Double& , Type_Double&  ) const;
  template <typename Type_Double> inline void coeffs_face(const int, const int, const Periodique&, Type_Double& , Type_Double&  ) const;
  template <typename Type_Double> inline void coeffs_faces_interne(int, Type_Double& , Type_Double&  ) const;

  template <typename Type_Double> inline void secmem_face(const int, const Symetrie&, const int, Type_Double& ) const;
  template <typename Type_Double> inline void secmem_face(const int, const Neumann_sortie_libre&, const int, Type_Double& ) const;
  template <typename Type_Double> inline void secmem_face(const int, const Dirichlet_entree_fluide&, const int, Type_Double& ) const;
  template <typename Type_Double> inline void secmem_face(const int, const Dirichlet_paroi_fixe&, const int, Type_Double& ) const;
  template <typename Type_Double> inline void secmem_face(const int, const Dirichlet_paroi_defilante&, const int, Type_Double& ) const;
  template <typename Type_Double> inline void secmem_face(const int, const Neumann_paroi_adiabatique&, const int, Type_Double& ) const;
  template <typename Type_Double> inline void secmem_face(const int, const Neumann_paroi&, const int, Type_Double& ) const;
  template <typename Type_Double> inline void secmem_face(const int, const int, const int, const Echange_externe_impose&, const int, Type_Double& ) const;
  template <typename Type_Double> inline void secmem_face(const int, const Echange_global_impose&, const int, Type_Double& ) const;
  template <typename Type_Double> inline void secmem_face(const int, const Periodique&, const int, Type_Double& ) const;
  template <typename Type_Double> inline void secmem_faces_interne(const int, Type_Double& ) const;
};

//
// Fonctions inline de la classe Eval_Diff_K_Eps_QC_VDF_Elem
//

inline Eval_Diff_K_Eps_QC_VDF_Elem::Eval_Diff_K_Eps_QC_VDF_Elem() : Eval_Diff_K_Eps_VDF() {}
// DEBUT DES DEFINES
#define CLASSNAME Eval_Diff_K_Eps_QC_VDF_Elem
#define nu_1(i,k) (db_diffusivite+dv_diffusivite_turbulente(i)/Prdt[k])
// normalement nu2 ne sert pas!!
#define nu_2(i,k) 1.;assert(0);Process::exit();
#define f_heq(d0,i,d1,j,k) heq=0.5*(nu_1(i,k) + nu_1(j,k))/(d1+d0);
#undef D_AXI
#undef DEQUIV
#undef MULTD
#define ISQUASI
#include <Vect_corps_base.h>
#undef CLASSNAME
#undef f_heq
#undef nu_1
#undef nu_2
#undef DEQUIV
#undef MULTD
#undef D_AXI
#undef Dist_face_elem0
#undef Dist_face_elem1
#undef Dist_norm_bord_externe
#undef Dist_norm_bord
#undef MODIF_DEQ
#undef ISQUASI
#endif
