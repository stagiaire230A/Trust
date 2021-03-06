/****************************************************************************
* Copyright (c) 2019, CEA
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
// File:        Source_Transport_K_Eps_Bas_Reynolds_W_VDF_Elem.cpp
// Directory:   $TRUST_ROOT/src/VDF/Turbulence
// Version:     /main/22
//
//////////////////////////////////////////////////////////////////////////////

#include <Source_Transport_K_Eps_Bas_Reynolds_W_VDF_Elem.h>
#include <Modele_turbulence_scal_Fluctuation_Temperature_W_Bas_Re.h>
#include <Convection_Diffusion_Temperature.h>
#include <Probleme_base.h>
#include <IntTrav.h>
#include <Entree_fluide_temperature_imposee.h>
#include <Champ_Uniforme.h>
#include <Champ_Face.h>
#include <Zone_VDF.h>
#include <Zone_Cl_VDF.h>
#include <Modele_turbulence_hyd_K_Eps_Bas_Reynolds.h>
#include <DoubleTrav.h>
#include <Fluide_Quasi_Compressible.h>

Implemente_instanciable_sans_constructeur(Source_Transport_K_Eps_Bas_Reynolds_W_VDF_Elem,"Source_Transport_K_Eps_Bas_Reynolds_W_VDF_P0_VDF",Source_Transport_K_Eps_VDF_Elem);

Implemente_instanciable_sans_constructeur(Source_Transport_K_Eps_Bas_Reynolds_anisotherme_W_VDF_Elem,"Source_Transport_K_Eps_Bas_Reynolds_anisotherme_W_VDF_P0_VDF",Source_Transport_K_Eps_Bas_Reynolds_W_VDF_Elem);




//// printOn
//

Sortie& Source_Transport_K_Eps_Bas_Reynolds_W_VDF_Elem::printOn(Sortie& s ) const
{
  return s << que_suis_je() ;
}

Sortie& Source_Transport_K_Eps_Bas_Reynolds_anisotherme_W_VDF_Elem::printOn(Sortie& s) const
{
  return s << que_suis_je() ;
}



//// readOn
//

Entree& Source_Transport_K_Eps_Bas_Reynolds_W_VDF_Elem::readOn(Entree& is )
{
  Motcle accolade_ouverte("{");
  Motcle accolade_fermee("}");
  Motcle motlu;

  is >> motlu;
  if (motlu != accolade_ouverte)
    {
      Cerr << "On attendait { pour commencer a lire les constantes de Source_Transport_K_Eps_Bas_Reynolds_W" << finl;
      exit();
    }
  Cerr << "Lecture des constantes de Source_Transport_K_Eps_Bas_Reynolds_W" << finl;
  Motcles les_mots(2);
  {
    les_mots[0] = "C1_eps";
    les_mots[1] = "C2_eps";
  }
  is >> motlu;
  while (motlu != accolade_fermee)
    {
      int rang=les_mots.search(motlu);
      switch(rang)
        {
        case 0 :
          {
            is >> C1;
            break;
          }
        case 1 :
          {
            is >> C2;
            break;
          }
        default :
          {
            Cerr << "On ne comprend pas le mot cle : " << motlu << "dans Source_Transport_K_Eps_Bas_Reynolds_W" << finl;
            exit();
          }
        }

      is >> motlu;
    }
  return is;
}

Entree& Source_Transport_K_Eps_Bas_Reynolds_anisotherme_W_VDF_Elem::readOn(Entree& s)
{
  return s ;
}





/////////////////////////////////////////////////////////////////////////////
//
//            Implementation des fonctions de la classe
//
//             Source_Transport_K_Eps_Bas_Reynolds_W_VDF_Elem
//
/////////////////////////////////////////////////////////////////////////////

void Source_Transport_K_Eps_Bas_Reynolds_W_VDF_Elem::associer_pb(const Probleme_base& pb )
{
  eq_hydraulique = pb.equation(0);
  eqn_keps_bas_re = ref_cast(Transport_K_Eps_Bas_Reynolds,equation());
}

void Source_Transport_K_Eps_Bas_Reynolds_W_VDF_Elem::associer_zones(const Zone_dis& zone_dis, const Zone_Cl_dis& zone_Cl_dis)
{
  la_zone_VDF = ref_cast(Zone_VDF, zone_dis.valeur());
  la_zone_Cl_VDF = ref_cast(Zone_Cl_VDF, zone_Cl_dis.valeur());
}

DoubleTab& Source_Transport_K_Eps_Bas_Reynolds_W_VDF_Elem::ajouter(DoubleTab& resu) const
{

  const Zone_Cl_dis& zcl=eq_hydraulique->zone_Cl_dis();
  const Zone_dis& z = eq_hydraulique->zone_dis();
  const Zone_VDF& zone_VDF = la_zone_VDF.valeur();
  const Zone_Cl_VDF& zone_Cl_VDF = la_zone_Cl_VDF.valeur();
  const  DoubleTab& K_eps_Bas_Re = eqn_keps_bas_re->inconnue().valeurs();
  const DoubleTab& visco_turb = eqn_keps_bas_re->modele_turbulence().viscosite_turbulente().valeurs();
  const Modele_turbulence_hyd_K_Eps_Bas_Reynolds& mod_turb = ref_cast(Modele_turbulence_hyd_K_Eps_Bas_Reynolds,eqn_keps_bas_re->modele_turbulence());
  const Modele_Fonc_Bas_Reynolds& mon_modele_fonc = mod_turb.associe_modele_fonction();
  const Fluide_base& fluide = ref_cast(Fluide_base,eq_hydraulique->milieu());
  const Champ_Don& ch_visco_cin = fluide.viscosite_cinematique();
  /*
    const DoubleTab& tab_visco = ch_visco_cin->valeurs();
    double visco;
    if (sub_type(Champ_Uniforme,ch_visco_cin.valeur()))
    {
    visco = max(tab_visco(0,0),DMINFLOAT);
    }
    else {assert(0);exit();visco=-1;}
  */
  const DoubleTab& vit = eq_hydraulique->inconnue().valeurs();
  const DoubleVect& volumes = zone_VDF.volumes();
  const DoubleVect& porosite_vol = zone_VDF.porosite_elem();
  int nb_elem = zone_VDF.nb_elem();
  //  int nb_elem_tot = zone_VDF.nb_elem_tot();
  DoubleTrav P(nb_elem);
  DoubleTrav D(nb_elem);
  DoubleTrav E(nb_elem);
  DoubleTrav F1(nb_elem);
  DoubleTrav F2(nb_elem);
  // Cerr << "le modele fonc est " << mon_modele_fonc.valeur().que_suis_je() << finl;

  mon_modele_fonc.Calcul_D(D,z,zcl,vit,K_eps_Bas_Re,ch_visco_cin);
  mon_modele_fonc.Calcul_E(E,z,zcl,vit,K_eps_Bas_Re,ch_visco_cin,visco_turb);
  //mon_modele_fonc.Calcul_F1(F1,z);
  mon_modele_fonc.Calcul_F2(F2,D,z,K_eps_Bas_Re,ch_visco_cin );
  if (axi)
    {
      Champ_Face& vitesse = ref_cast_non_const(Champ_Face,eq_hydraulique->inconnue().valeur());
      calculer_terme_production_K_Axi(zone_VDF,vitesse,P,K_eps_Bas_Re,visco_turb);
    }
  else
    {
      Champ_Face& vitesse = ref_cast_non_const(Champ_Face,eq_hydraulique->inconnue().valeur());
      calculer_terme_production_K(zone_VDF,zone_Cl_VDF ,P,K_eps_Bas_Re,vit,vitesse,visco_turb);
    }

  for (int elem=0; elem<nb_elem; elem++)
    {
      resu(elem,0) += (P(elem)-K_eps_Bas_Re(elem,1)-D(elem))*volumes(elem)*porosite_vol(elem);

      //   Cerr << "K = " << K_eps_Bas_Re(elem,0) << finl;
      if (K_eps_Bas_Re(elem,0) >= 10.e-10)
        resu(elem,1) += (C1*F1(elem)*P(elem)- C2*F2(elem)*K_eps_Bas_Re(elem,1))*volumes(elem)*porosite_vol(elem)
                        *K_eps_Bas_Re(elem,1)/K_eps_Bas_Re(elem,0)+E(elem)*volumes(elem)*porosite_vol(elem);
    }
  //   Cerr << "resu = " << resu(elem,1) << finl;
  return resu;
}

DoubleTab& Source_Transport_K_Eps_Bas_Reynolds_W_VDF_Elem::calculer(DoubleTab& resu) const
{
  resu = 0.;
  return ajouter(resu);
}

/////////////////////////////////////////////////////////////////////////////
//
//            Implementation des fonctions de la classe
//
//             Source_Transport_K_Eps_Bas_Reynolds_anisotherme_W_VDF_Elem
//
/////////////////////////////////////////////////////////////////////////////
//



/************************************************/
//Fonctions qui calculent le terme g*beta*teta^2
/************************************************/
DoubleTab& Source_Transport_K_Eps_Bas_Reynolds_anisotherme_W_VDF_Elem::calculer_gteta2(const Zone_VDF& zone_VDF, DoubleTab& gteta2 ,const DoubleTab& fluctu_temp, double beta,const DoubleVect& gravite) const
{
  //
  // gteta2 est discretise au centre des elements
  //

  int nb_elem = zone_VDF.nb_elem();
  int nb_faces= zone_VDF.nb_faces();
  DoubleTrav u_teta(nb_faces);
  //  const DoubleVect& porosite_face = zone_VDF.porosite_face();
  gteta2 = 0;

  //                ------->  -------->
  // Calcul de beta.gravite . tetacarre

  const Zone& la_zone=zone_VDF.zone();
  int nb_faces_elem = la_zone.nb_faces_elem();

  IntTrav numfa(nb_faces_elem);
  DoubleVect coef(dimension);
  //  const IntTab& les_elem_faces = zone_VDF.elem_faces();

  for (int elem=0; elem<nb_elem; elem++)
    for (int dim=0; dim<dimension; dim++)
      gteta2(elem,dim) = beta*gravite(dim)*fluctu_temp(elem,0) ;
  return gteta2;
}

DoubleTab& Source_Transport_K_Eps_Bas_Reynolds_anisotherme_W_VDF_Elem::calculer_gteta2(const Zone_VDF& zone_VDF,DoubleTab& gteta2 ,const DoubleTab& fluctu_temp,const DoubleTab& beta,const DoubleVect& gravite) const
{
  //
  // gteta2 est discretise au centre des elements
  //

  int nb_elem = zone_VDF.nb_elem();
  int nb_faces= zone_VDF.nb_faces();
  DoubleTrav u_teta(nb_faces);
  //  const DoubleVect& porosite_face = zone_VDF.porosite_face();
  gteta2 = 0;

  //                ------->  -------->
  // Calcul de beta.gravite . tetacarre

  const Zone& la_zone=zone_VDF.zone();
  int nb_faces_elem = la_zone.nb_faces_elem();

  IntTrav numfa(nb_faces_elem);
  DoubleVect coef(dimension);
  //  const IntTab& les_elem_faces = zone_VDF.elem_faces();

  for (int elem=0; elem<nb_elem; elem++)
    for (int dim=0; dim<dimension; dim++)
      gteta2(elem,dim) = beta(elem)*gravite(dim)*fluctu_temp(elem,0) ;
  return gteta2;
}


DoubleTab& Source_Transport_K_Eps_Bas_Reynolds_anisotherme_W_VDF_Elem::calculer_u_teta_W(const Zone_VDF& zone_VDF,
                                                                                         const Zone_Cl_VDF& zcl_VDF,
                                                                                         const DoubleTab& temper,const DoubleTab& fluctu_temp,                                                                    const DoubleTab& keps,
                                                                                         const DoubleTab& alpha_turb,
                                                                                         DoubleTab& u_teta) const
{
  //                                      ---->
  // On note u_teta le vecteur alpha_turb.gradT
  //
  // Sur chaque face on calcule la composante de u_teta normale a la face

  int nb_faces= zone_VDF.nb_faces();
  int n0,n1,n_bord;
  double alpha,dist;
  int face;
  int nb_elem_tot = zone_VDF.nb_elem_tot();
  const IntTab& face_voisins = zone_VDF.face_voisins();
  DoubleTab gteta2(nb_elem_tot,dimension);
  const IntVect& orientation = zone_VDF.orientation();
  u_teta = 0;

  // Traitement des faces internes
  int premiere_face_int=zone_VDF.premiere_face_int();
  nb_faces=zone_VDF.nb_faces();

  if (Objet_U::axi)

    for (face=premiere_face_int; face<nb_faces; face++)
      {
        n0 = face_voisins(face,0);
        n1 = face_voisins(face,1);
        dist = zone_VDF.dist_norm_axi(face);
        alpha = 0.5*(alpha_turb(n0)+alpha_turb(n1));
        u_teta[face] = alpha*(temper[n1] - temper[n0])/dist;
      }
  else

    for (face=premiere_face_int; face<nb_faces; face++)
      {
        n0 = face_voisins(face,0);
        n1 = face_voisins(face,1);
        dist = zone_VDF.dist_norm(face);
        alpha = 0.5*(alpha_turb(n0)+alpha_turb(n1));
        u_teta[face] = alpha*(temper[n1] - temper[n0])/dist;
      }

  // Traitement des conditions limites de type Entree_fluide_K_Eps_impose :

  for (n_bord=0; n_bord<zone_VDF.nb_front_Cl(); n_bord++)
    {

      const Cond_lim& la_cl = zcl_VDF.les_conditions_limites(n_bord);

      if (sub_type(Entree_fluide_temperature_imposee,la_cl.valeur()) )
        {
          const Entree_fluide_temperature_imposee& la_cl_diri=ref_cast(Entree_fluide_temperature_imposee,la_cl.valeur());
          const Front_VF& le_bord = ref_cast(Front_VF,la_cl.frontiere_dis());
          int ndeb = le_bord.num_premiere_face();
          int nfin = ndeb + le_bord.nb_faces();
          for (face=ndeb; face<nfin; face++)
            {
              double T_imp = la_cl_diri.val_imp(face-ndeb);
              n0 = face_voisins(face,0);
              n1 = face_voisins(face,1);
              if (Objet_U::axi)
                dist = 2*zone_VDF.dist_norm_bord_axi(face);
              else
                dist = 2*zone_VDF.dist_norm_bord(face);
              if (n0 != -1)
                {
                  alpha = alpha_turb(n0);
                  u_teta[face] = alpha*(T_imp-temper[n0])/dist;
                }
              else   // n1 != -1
                {
                  alpha = alpha_turb(n1);
                  u_teta[face] = alpha*(temper[n1]-T_imp)/dist;
                }
            }
        }
    }
  const DoubleTab& g = gravite_->valeurs();
  const Champ_Don& ch_beta = beta_t.valeur();
  const DoubleTab& tab_beta = ch_beta.valeurs();

  //on calcule gteta2 pour corriger u_teta confermement au modele de Wrobel
  if (sub_type(Champ_Uniforme,ch_beta.valeur()))
    calculer_gteta2(zone_VDF,gteta2 ,fluctu_temp,tab_beta(0,0),g);
  else
    calculer_gteta2(zone_VDF, gteta2 ,fluctu_temp,tab_beta,g);


  //faire ICI u_tet=utet-Cb*tau*g*Beta*theta2
  int ori,num_face,elem1,elem2;
  for (n_bord=0; n_bord<zone_VDF.nb_front_Cl(); n_bord++)
    {
      const Cond_lim& la_cl = zcl_VDF.les_conditions_limites(n_bord);
      const Front_VF& le_bord = ref_cast(Front_VF,la_cl.frontiere_dis());
      int ndeb = le_bord.num_premiere_face();
      int nfin = ndeb + le_bord.nb_faces();
      for (num_face=ndeb; num_face<nfin; num_face++)
        {
          ori = orientation(num_face);
          elem1 = face_voisins(num_face,0);
          if (elem1 != -1)
            {
              if ( (keps(elem1,0)>1.e-10) && (keps(elem1,1)>1.e-10) && (fluctu_temp(elem1,1)>1.e-10) && (fluctu_temp(elem1,0)>1.e-10))
                {
                  double tau = sqrt ( keps(elem1,0)/keps(elem1,1) * fluctu_temp(elem1,0)/fluctu_temp(elem1,1)/2 );
                  u_teta(num_face)=u_teta(num_face)-0.7*tau*gteta2(elem1,ori);
                }
            }
          else
            {
              elem2 = face_voisins(num_face,1);
              if ( (keps(elem2,0)>1.e-10) && (keps(elem2,1)>1.e-10) && (fluctu_temp(elem2,1)>1.e-10) && (fluctu_temp(elem2,0)>1.e-10))
                {
                  double tau = sqrt ( keps(elem2,0)/keps(elem2,1) *         fluctu_temp(elem2,0)/fluctu_temp(elem2,1)/2 );
                  u_teta(num_face)=u_teta(num_face)-0.7*tau*gteta2(elem2,ori);
                }
            }
        }
    }

  // traitement des faces internes
  for (num_face=zone_VDF.premiere_face_int(); num_face<nb_faces; num_face++)
    {
      ori = orientation(num_face);
      elem1 = face_voisins(num_face,0);
      elem2 = face_voisins(num_face,1);

      double gtet = 0.5 *( gteta2(elem1,ori) + gteta2(elem2,ori) );

      if ( (keps(elem1,1)>1.e-10) && (fluctu_temp(elem1,1)>1.e-10) && (keps(elem2,1)>1.e-10) && (fluctu_temp(elem2,1)>1.e-10) && (keps(elem1,0)>1.e-10) && (fluctu_temp(elem1,0)>1.e-10) && (keps(elem2,0)>1.e-10) && (fluctu_temp(elem2,0)>1.e-10))
        {
          double tau =0.5* ( sqrt ( keps(elem1,0)/keps(elem1,1) * fluctu_temp(elem1,0)/fluctu_temp(elem1,1)/2)  + sqrt(keps(elem2,0)/keps(elem2,1) * fluctu_temp(elem2,0)/fluctu_temp(elem2,1)/2) );
          u_teta(num_face)=u_teta(num_face)-0.7*tau*gtet;
        }
    }

  return u_teta;
}

DoubleTab& Source_Transport_K_Eps_Bas_Reynolds_anisotherme_W_VDF_Elem::calculer_terme_destruction_K_W(const Zone_VDF& zone_VDF,
                                                                                                      const Zone_Cl_VDF& zcl_VDF,
                                                                                                      DoubleTab& G,const DoubleTab& temper,
                                                                                                      const DoubleTab& fluctuTemp,
                                                                                                      const DoubleTab& keps,
                                                                                                      const DoubleTab& alpha_turb,
                                                                                                      double beta,const DoubleVect& gravite) const
{
  //
  // G est discretise comme K et Eps i.e au centre des elements
  //
  //                                       --> ----->
  // G(elem) = beta alpha_t(elemn) G . gradT(elem)
  //

  int nb_elem = zone_VDF.nb_elem();
  int nb_faces= zone_VDF.nb_faces();
  DoubleTrav u_teta(nb_faces);
  const DoubleVect& porosite_face = zone_VDF.porosite_face();

  //                                      ---->
  // On note u_teta le vecteur alpha_turb.gradT
  //
  // Appel a la fonction qui calcule sur chaque face la composante
  // de u_teta normale a la face

  calculer_u_teta_W(zone_VDF,zcl_VDF,temper,fluctuTemp,keps,alpha_turb,u_teta);

  //                                          ------> ----->
  // On calcule ensuite une valeur moyenne de gravite.u_teta sur chaque
  // element.

  G = 0;

  //                ------->  ------>
  // Calcul de beta.gravite . u_teta

  const Zone& la_zone=zone_VDF.zone();
  int nb_faces_elem = la_zone.nb_faces_elem();

  IntTrav numfa(nb_faces_elem);
  DoubleVect coef(Objet_U::dimension);
  const IntTab& les_elem_faces = zone_VDF.elem_faces();

  for (int elem=0; elem<nb_elem; elem++)
    {
      for (int i=0; i<nb_faces_elem; i++)
        numfa[i] = les_elem_faces(elem,i);

      coef(0) = 0.5*(u_teta(numfa[0])*porosite_face(numfa[0])
                     + u_teta(numfa[dimension])*porosite_face(numfa[dimension]));
      coef(1) = 0.5*(u_teta(numfa[1])*porosite_face(numfa[1])
                     + u_teta(numfa[dimension+1])*porosite_face(numfa[dimension+1]));

      if (dimension == 2)
        G[elem] = beta*(gravite(0)*coef(0) + gravite(1)*coef(1));

      else if (dimension == 3)
        {
          coef(2) = 0.5*(u_teta(numfa[2])*porosite_face(numfa[2])
                         + u_teta(numfa[5])*porosite_face(numfa[5]));
          G[elem] = beta*(gravite(0)*coef(0) + gravite(1)*coef(1) + gravite(2)*coef(2));
        }

    }

  return G;
}

DoubleTab& Source_Transport_K_Eps_Bas_Reynolds_anisotherme_W_VDF_Elem::calculer_terme_destruction_K_W(const Zone_VDF& zone_VDF,
                                                                                                      const Zone_Cl_VDF& zcl_VDF,
                                                                                                      DoubleTab& G,const DoubleTab& temper,
                                                                                                      const DoubleTab& fluctuTemp,
                                                                                                      const DoubleTab& keps,
                                                                                                      const DoubleTab& alpha_turb,
                                                                                                      const DoubleTab& beta,const DoubleVect& gravite) const
{
  //
  // G est discretise comme K et Eps i.e au centre des elements
  //
  //                                       --> ----->
  // G(elem) = beta(elem) alpha_t(elem) G . gradT(elem)
  //

  int nb_elem = zone_VDF.nb_elem();
  int nb_faces= zone_VDF.nb_faces();
  DoubleTrav u_teta(nb_faces);
  const DoubleVect& porosite_face = zone_VDF.porosite_face();

  //                                      ---->
  // On note u_teta le vecteur alpha_turb.gradT
  //
  // Appel a la fonction qui calcule sur chaque face la composante
  // de u_teta normale a la face

  calculer_u_teta_W(zone_VDF,zcl_VDF,temper,fluctuTemp,keps,alpha_turb,u_teta);

  //                                          ------> ----->
  // On calcule ensuite une valeur moyenne de gravite.u_teta sur chaque
  // element.

  G = 0;

  //                ------->  ------>
  // Calcul de beta.gravite . u_teta

  const Zone& la_zone=zone_VDF.zone();
  int nb_faces_elem = la_zone.nb_faces_elem();
  IntTrav numfa(nb_faces_elem);
  const IntTab& les_elem_faces = zone_VDF.elem_faces();
  DoubleVect coef(dimension);

  for (int elem=0; elem<nb_elem; elem++)
    {
      for (int i=0; i<nb_faces_elem; i++)
        numfa[i] = les_elem_faces(elem,i);

      coef(0) = 0.5*(u_teta(numfa[0])*porosite_face(numfa[0])
                     + u_teta(numfa[dimension])*porosite_face(numfa[dimension]));
      coef(1) = 0.5*(u_teta(numfa[1])*porosite_face(numfa[1])
                     + u_teta(numfa[dimension+1])*porosite_face(numfa[dimension+1]));

      if (dimension ==2)
        G[elem] = beta(elem)*(gravite(0)*coef(0) + gravite(1)*coef(1));

      else if (dimension == 3)
        {
          coef(2) = 0.5*(u_teta(numfa[2])*porosite_face(numfa[2])
                         + u_teta(numfa[5])*porosite_face(numfa[5]));
          G[elem] = beta(elem)*(gravite(0)*coef(0) + gravite(1)*coef(1) + gravite(2)*coef(2));
        }

    }

  return G;
}

// remplit les references eq_thermique et beta

void Source_Transport_K_Eps_Bas_Reynolds_anisotherme_W_VDF_Elem::associer_pb(const Probleme_base& pb)
{
  if (pb.nombre_d_equations()<2)
    {
      Cerr<<"The K_Eps source term "<<que_suis_je()<<" cannot be activated"<<finl;
      Cerr<<"for a "<<pb.que_suis_je()<<" problem."<<finl;
    }
  const Equation_base& eqn = pb.equation(1);
  const Milieu_base& milieu = eqn.milieu();
  const Fluide_base& fluide = ref_cast(Fluide_base,milieu);

  if (sub_type(Fluide_Quasi_Compressible,fluide))
    {
      Cerr<<"The K_Eps source term "<<que_suis_je()<<" cannot be activated"<<finl;
      Cerr<<"with a "<<milieu.que_suis_je()<<" medium."<<finl;
      exit();
    }

  Source_Transport_K_Eps_Bas_Reynolds_W_VDF_Elem::associer_pb(pb);

  const Convection_Diffusion_Temperature& eqn_th =
    ref_cast(Convection_Diffusion_Temperature,pb.equation(1));
  eq_thermique = eqn_th;
  //const Fluide_base& fluide = eq_thermique->fluide();
  beta_t = fluide.beta_t();
  gravite_ = fluide.gravite();
}

DoubleTab& Source_Transport_K_Eps_Bas_Reynolds_anisotherme_W_VDF_Elem::ajouter(DoubleTab& resu) const
{
  const Zone_Cl_dis& zcl=eq_hydraulique->zone_Cl_dis();
  const Zone_dis& z = eq_hydraulique->zone_dis();
  const Zone_VDF& zone_VDF = ref_cast(Zone_VDF,eq_hydraulique->zone_dis().valeur());
  const Zone_Cl_VDF& zcl_VDF = ref_cast(Zone_Cl_VDF,eq_hydraulique->zone_Cl_dis().valeur());
  const Zone_Cl_VDF& zcl_VDF_th = ref_cast(Zone_Cl_VDF,eq_thermique->zone_Cl_dis().valeur());
  const DoubleTab& K_eps_Bas_Re = eqn_keps_bas_re->inconnue().valeurs();
  const DoubleTab& scalaire = eq_thermique->inconnue().valeurs();
  const DoubleTab& vit = eq_hydraulique->inconnue().valeurs();
  const DoubleTab& visco_turb = eqn_keps_bas_re->modele_turbulence().viscosite_turbulente().valeurs();
  const Modele_turbulence_scal_base& le_modele_scalaire = ref_cast(Modele_turbulence_scal_base,eq_thermique->get_modele(TURBULENCE).valeur());
  const Modele_turbulence_scal_Fluctuation_Temperature_W& modele_Fluctu = ref_cast(Modele_turbulence_scal_Fluctuation_Temperature_W,le_modele_scalaire);
  const DoubleTab& alpha_turb = le_modele_scalaire.diffusivite_turbulente().valeurs();
  const Transport_Fluctuation_Temperature_W& monEqFluctu = modele_Fluctu.equation_Fluctu();
  const DoubleTab& Fluctu_Temperature = monEqFluctu.inconnue().valeurs();
  const DoubleTab& g = gravite_->valeurs();
  const Champ_Don& ch_beta = beta_t.valeur();
  const DoubleVect& volumes = zone_VDF.volumes();
  const DoubleVect& porosite_vol = zone_VDF.porosite_elem();
  const Fluide_base& fluide = ref_cast(Fluide_base,eq_hydraulique->milieu());
  const Champ_Don& ch_visco_cin = fluide.viscosite_cinematique();
  /*
    const DoubleTab& tab_visco = ch_visco_cin->valeurs();
  */
  const Modele_turbulence_hyd_K_Eps_Bas_Reynolds& mod_turb = ref_cast(Modele_turbulence_hyd_K_Eps_Bas_Reynolds,eqn_keps_bas_re->modele_turbulence());
  const Modele_Fonc_Bas_Reynolds& mon_modele_fonc = ref_cast(Modele_Fonc_Bas_Reynolds,mod_turb.associe_modele_fonction());
  int nb_elem = zone_VDF.nb_elem();
  int nb_elem_tot = zone_VDF.nb_elem_tot();
  //  int nb_faces= zone_VDF.nb_faces();
  //  const DoubleVect& porosite_face = zone_VDF.porosite_face();
  DoubleTrav P(nb_elem_tot);
  DoubleTrav G(nb_elem_tot);
  DoubleTrav D(nb_elem_tot);
  DoubleTrav E(nb_elem_tot);
  DoubleTrav F1(nb_elem_tot);
  DoubleTrav F2(nb_elem_tot);
  mon_modele_fonc.Calcul_D(D,z,zcl,vit,K_eps_Bas_Re,ch_visco_cin);
  // Cerr << "D = " << D << finl;
  mon_modele_fonc.Calcul_E(E,z,zcl, vit,K_eps_Bas_Re,ch_visco_cin,visco_turb);
  // Cerr << "E = " << E << finl;
  //mon_modele_fonc.Calcul_F1(F1,z);
  // Cerr << "F1 = " << F1 << finl;
  mon_modele_fonc.Calcul_F2(F2,D,z,K_eps_Bas_Re,ch_visco_cin);
  // Cerr << "F2 = " << F2 << finl;

  if (axi)
    {
      Champ_Face& vitesse = ref_cast_non_const(Champ_Face,eq_hydraulique->inconnue().valeur());
      calculer_terme_production_K_Axi(zone_VDF,vitesse,P,K_eps_Bas_Re,visco_turb);
    }
  else
    {
      Champ_Face& vitesse = ref_cast_non_const(Champ_Face,eq_hydraulique->inconnue().valeur());
      calculer_terme_production_K(zone_VDF,zcl_VDF,P,K_eps_Bas_Re,vit,vitesse,visco_turb);
    }

  // C'est l'objet de type zone_Cl_dis de l'equation thermique
  // qui est utilise dans le calcul de G

  const DoubleTab& tab_beta = ch_beta.valeurs();
  /*
     if (sub_type(Champ_Uniforme,ch_beta.valeur()))
     calculer_terme_destruction_K(zone_VDF,zcl_VDF_th,G,scalaire,alpha_turb,tab_beta(0,0),g);
     else
     calculer_terme_destruction_K(zone_VDF,zcl_VDF_th,G,scalaire,alpha_turb,tab_beta,g);
  */

  if (sub_type(Champ_Uniforme,ch_beta.valeur()))
    calculer_terme_destruction_K_W(zone_VDF,zcl_VDF_th,G,scalaire,Fluctu_Temperature,K_eps_Bas_Re,alpha_turb,tab_beta(0,0),g);
  else
    calculer_terme_destruction_K_W(zone_VDF,zcl_VDF_th,G,scalaire,Fluctu_Temperature,K_eps_Bas_Re,alpha_turb,tab_beta,g);

  for (int elem=0; elem<nb_elem; elem++)
    {
      resu(elem,0) += (P(elem)-K_eps_Bas_Re(elem,1)-D(elem))*volumes(elem)*porosite_vol(elem);
      if (K_eps_Bas_Re(elem,0) >= 1.e-10)
        {
          resu(elem,1) += (C1*F1(elem)*P(elem)- C2*F2(elem)*K_eps_Bas_Re(elem,1))*volumes(elem)*porosite_vol(elem) *(K_eps_Bas_Re(elem,1)/K_eps_Bas_Re(elem,0)) + E(elem)*volumes(elem)*porosite_vol(elem);
        }
      if ( (G(elem)>0) && (K_eps_Bas_Re(elem,0) >= 1.e-8) )
        {
          resu(elem,0) += G(elem)*volumes(elem)*porosite_vol(elem);
          resu(elem,1) += C1*F1(elem)*G(elem)*K_eps_Bas_Re(elem,1)*volumes(elem)*porosite_vol(elem)
                          /K_eps_Bas_Re(elem,0);
        }
    }
  // Cerr << "Source_Transport_K_Eps_Bas_Reynolds_anisotherme_W_VDF_Elem::ajouter ok" << finl;

  return resu;
}

DoubleTab& Source_Transport_K_Eps_Bas_Reynolds_anisotherme_W_VDF_Elem::calculer(DoubleTab& resu) const
{

  resu = 0.;
  return ajouter(resu);

}
