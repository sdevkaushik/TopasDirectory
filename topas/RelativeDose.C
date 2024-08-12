//ROOT program to calculate relative dose deposition by secondary particles assuming local approximation (i.e. electronic KERMA(fEnergy) = absorbed dose)

#include<iostream>
#include <fstream>
#include "TROOT.h"
#include "TEventList.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TStyle.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TPad.h"


int RelativeDose()
{

  // Open the ROOT file containing the TTree
  
  //TFile *f = new TFile("/home/sdev/TopasDirectory/topas/output_files/1keV_outputFiles/1keV_output.root");
  //TFile *f = new TFile("/home/sdev/TopasDirectory/topas/output_files/1keV_outputFiles/topas_1keV_TsNtupleForCellNew.root");
  //TFile *f = new TFile("/home/sdev/TopasDirectory/topas/output_files/1MeV_outputFiles/1MeV_output.root");
  //TFile *f = new TFile("/home/sdev/TopasDirectory/topas/output_files/1MeV_outputFiles/topas_1MeV_WithStopping_TsNtupleForCellNew.root");
  TFile *f = new TFile("/home/sdev/TopasDirectory/topas/output_files/1MeV_outputFiles/topas_1MeV_TsNtupleForCell.root");
  //TFile *f = new TFile("/home/sdev/TopasDirectory/topas/output_files/1MeV_outputFiles/10MeV_output.root");
  //TFile *f = new TFile("/home/sdev/TopasDirectory/topas/output_files/RealNeutronSpectra_LocationA/LocationA_output.root");
  //TFile *f = new TFile("/home/sdev/TopasDirectory/topas/output_files/RealNeutronSpectra_LocationC/LocationC_output.root");
  //TFile *f = new TFile("/home/sdev/TopasDirectory/topas/output_files/RealNeutronSpectra_LocationD/LocationD_output.root");

   
  // Extract the TTree from the file
  TTree *treeInner = (TTree*)f->Get("InnerScorer");

  TTree *treeInter = (TTree*)f->Get("IntermediateScorer");

  TTree *treeOuter = (TTree*)f->Get("OuterScorer");

  // Define variables to hold the data from the TTree branches
  float  posX, posY, posZ, EnergyMeV, EnergyDepositedMeV;
  int PDG,TrackID,RunID,EventID;
  char VolumeName;

  // Set up the branch addresses for the TTree_Inner
  treeInner->SetBranchAddress("Position_X__cm_", &posX);
  treeInner->SetBranchAddress("Position_Y__cm_", &posY);
  treeInner->SetBranchAddress("Position_Z__cm_", &posZ);
  treeInner->SetBranchAddress("Energy__MeV_", &EnergyMeV);
  treeInner->SetBranchAddress("Energy_Deposited__MeV_", &EnergyDepositedMeV);
  treeInner->SetBranchAddress("Particle_Type__in_PDG_Format_", &PDG);
  treeInner->SetBranchAddress("Track_ID", &TrackID);
  treeInner->SetBranchAddress("Run_ID", &RunID);
  treeInner->SetBranchAddress("Event_ID", &EventID);
  treeInner->SetBranchAddress("Volume_Name", &VolumeName);
   
  // Set up the branch addresses for the TTree_Inter
  treeInter->SetBranchAddress("Position_X__cm_", &posX);
  treeInter->SetBranchAddress("Position_Y__cm_", &posY);
  treeInter->SetBranchAddress("Position_Z__cm_", &posZ);
  treeInter->SetBranchAddress("Energy__MeV_", &EnergyMeV);
  treeInter->SetBranchAddress("Energy_Deposited__MeV_", &EnergyDepositedMeV);
  treeInter->SetBranchAddress("Particle_Type__in_PDG_Format_", &PDG);
  treeInter->SetBranchAddress("Track_ID", &TrackID);
  treeInter->SetBranchAddress("Run_ID", &RunID);
  treeInter->SetBranchAddress("Event_ID", &EventID);
  treeInter->SetBranchAddress("Volume_Name", &VolumeName);

  // Set up the branch addresses for the TTree_Outer
  treeOuter->SetBranchAddress("Position_X__cm_", &posX);
  treeOuter->SetBranchAddress("Position_Y__cm_", &posY);
  treeOuter->SetBranchAddress("Position_Z__cm_", &posZ);
  treeOuter->SetBranchAddress("Energy__MeV_", &EnergyMeV);
  treeOuter->SetBranchAddress("Energy_Deposited__MeV_", &EnergyDepositedMeV);
  treeOuter->SetBranchAddress("Particle_Type__in_PDG_Format_", &PDG);
  treeOuter->SetBranchAddress("Track_ID", &TrackID);
  treeOuter->SetBranchAddress("Run_ID", &RunID);
  treeOuter->SetBranchAddress("Event_ID", &EventID);
  treeOuter->SetBranchAddress("Volume_Name", &VolumeName);
 

  int prev_TrackID = -1, prev_EventID = -1;

  double TotalDoseDepositedInner = 0;
  double TotalDoseDepositedInter = 0;
  double TotalDoseDepositedOuter = 0;

  double TotalPreStepKEInner = 0;
  double TotalPreStepKEInter = 0;
  double TotalPreStepKEOuter = 0;

  double TotalPostStepKEInner = 0;
  double TotalPostStepKEInter = 0;
  double TotalPostStepKEOuter = 0;

  
  double DoseDepositedProtonInner = 0;
  double DoseDepositedDeuteronInner = 0;
  double DoseDepositedTritonInner = 0;
  double DoseDepositedAlphaInner = 0;
  double DoseDepositedBerylliumInner = 0;
  double DoseDepositedBoronInner = 0;
  double DoseDepositedCarbonInner = 0;
  double DoseDepositedNitrogenInner = 0;
  double DoseDepositedOxygenInner = 0;
  double DoseDepositedElectronInner = 0;
  double DoseDepositedPositronInner = 0;
 
  double PreStepKEProtonInner = 0;
  double PreStepKEDeuteronInner = 0;
  double PreStepKETritonInner = 0;
  double PreStepKEAlphaInner = 0;
  double PreStepKEBerylliumInner = 0;
  double PreStepKEBoronInner = 0;
  double PreStepKECarbonInner = 0;
  double PreStepKENitrogenInner = 0;
  double PreStepKEOxygenInner = 0;
  double PreStepKEElectronInner = 0;
  double PreStepKEPositronInner = 0;


  double PostStepKEProtonInner = 0;
  double PostStepKEDeuteronInner = 0;
  double PostStepKETritonInner = 0;
  double PostStepKEAlphaInner = 0;
  double PostStepKEBerylliumInner = 0;
  double PostStepKEBoronInner = 0;
  double PostStepKECarbonInner = 0;
  double PostStepKENitrogenInner = 0;
  double PostStepKEOxygenInner = 0;
  double PostStepKEElectronInner = 0;
  double PostStepKEPositronInner = 0;

  //std::ofstream out("proton_inner.txt");

   
  if (treeInner)
    {

      if (!f || f->IsZombie())
	{
	  std::cerr << "Error: could not open file " << std::endl;
	  return 1;
	}
       
      if (f->IsOpen())
	{
	  for (int i = 0; i < treeInner->GetEntries(); i++)
	    {
	      treeInner->GetEntry(i);
	       
	      if(PDG==2212)  //Protons 

		{
		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {
		    	    
		      DoseDepositedProtonInner = DoseDepositedProtonInner + EnergyDepositedMeV;
		      PreStepKEProtonInner = PreStepKEProtonInner + EnergyMeV;
		      PostStepKEProtonInner = PostStepKEProtonInner + (EnergyMeV - EnergyDepositedMeV) ;

		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		      //std::cout<<"Initial Kinetic Energy of Protons :"<<EnergyMeV<<std::endl;
		      //std::cout<<"Total Energy Deposited deposited by Protons :"<<EnergyDepositedMeV<<std::endl;
		      //std::cout<<"Post step Kinetic Energy of Protons :"<<EnergyMeV - EnergyDepositedMeV<<std::endl;
		      //out << "Initial KE(preStep) : "<<EnergyMeV<<" Energy Deposited :" <<EnergyDepositedMeV <<'\n'; 
		   
		    }
		}

	      
	      if(PDG==1000010020)  //Deuterons

		{
		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {
		      DoseDepositedDeuteronInner = DoseDepositedDeuteronInner + EnergyDepositedMeV;
		      PreStepKEDeuteronInner = PreStepKEDeuteronInner + EnergyMeV;
		      PostStepKEDeuteronInner = PostStepKEDeuteronInner + (EnergyMeV - EnergyDepositedMeV) ;

		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<"Initial Kinetic Energy of Deuterons :"<<EnergyMeV<<std::endl;
		      //std::cout<<"Total Energy Deposited deposited by Deuterons :"<<EnergyDepositedMeV<<std::endl;
		   
		    }
		}

	      	      if(PDG==1000010030)  //Tritons

		{
		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {
		      DoseDepositedTritonInner = DoseDepositedTritonInner + EnergyDepositedMeV;
		      PreStepKETritonInner = PreStepKETritonInner + EnergyMeV;
		      PostStepKETritonInner = PostStepKETritonInner + (EnergyMeV -EnergyDepositedMeV) ;

		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<"Initial Kinetic Energy of Tritons :"<<EnergyMeV<<std::endl;
		      //std::cout<<"Total Energy Deposited deposited by Tritons :"<<EnergyDepositedMeV<<std::endl;
		   
		    }
		}

			       
	      if(PDG==1000020040)   //alphas
		 
		{

		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {

		      DoseDepositedAlphaInner = DoseDepositedAlphaInner + EnergyDepositedMeV;
		      PreStepKEAlphaInner = PreStepKEAlphaInner + EnergyMeV;
		      PostStepKEAlphaInner = PostStepKEAlphaInner + (EnergyMeV -EnergyDepositedMeV) ;

		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		      //std::cout<<"Initial Kinetic Energy of Alphas :"<<EnergyMeV<<std::endl;
		      //std::cout<<"Total Energy Deposited deposited by Alphas :"<<EnergyDepositedMeV<<std::endl;

		    }
		}

	          if(PDG==1000040090 || PDG==1000040100)   //Beryllium
	     
		{

		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {

		      DoseDepositedBerylliumInner = DoseDepositedBerylliumInner + EnergyDepositedMeV;
		      PreStepKEBerylliumInner = PreStepKEBerylliumInner + EnergyMeV;
		      PostStepKEBerylliumInner = PostStepKEBerylliumInner + (EnergyMeV -EnergyDepositedMeV) ;

		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		      //std::cout<<"Initial Kinetic Energy of Beryllium :"<<EnergyMeV<<std::endl;
		      //std::cout<<"Total Energy Deposited deposited by Beryllium :"<<EnergyDepositedMeV<<std::endl;

		    }
		}

		      if(PDG==1000050100 || PDG==1000050110)   //Boron
	     
		{

		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {

		      DoseDepositedBoronInner = DoseDepositedBoronInner + EnergyDepositedMeV;
		      PreStepKEBoronInner = PreStepKEBoronInner + EnergyMeV;
		      PostStepKEBoronInner = PostStepKEBoronInner + (EnergyMeV -EnergyDepositedMeV) ;

		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		      //std::cout<<"Initial Kinetic Energy of Boron :"<<EnergyMeV<<std::endl;
		      //std::cout<<"Total Energy Deposited deposited by Boron :"<<EnergyDepositedMeV<<std::endl;

		    }
		}

	       
	      if(PDG==1000060120 || PDG==1000060130 || PDG==1000060140 || PDG==1000060150)   //Carbon
	     
		{

		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {

		      DoseDepositedCarbonInner = DoseDepositedCarbonInner + EnergyDepositedMeV;
		      PreStepKECarbonInner = PreStepKECarbonInner + EnergyMeV;
		      PostStepKECarbonInner = PostStepKECarbonInner + (EnergyMeV -EnergyDepositedMeV) ;

		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		      //std::cout<<"Initial Kinetic Energy of Carbon :"<<EnergyMeV<<std::endl;
		      //std::cout<<"Total Energy Deposited deposited by Carbon :"<<EnergyDepositedMeV<<std::endl;

		    }
		}

  	       
	      if(PDG==1000070140 || PDG==1000070150 || PDG==1000070160)   //Nitrogen
		 
		{

		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {
		
		      DoseDepositedNitrogenInner = DoseDepositedNitrogenInner + EnergyDepositedMeV;
		      PreStepKENitrogenInner = PreStepKENitrogenInner + EnergyMeV;
		      PostStepKENitrogenInner = PostStepKENitrogenInner + (EnergyMeV -EnergyDepositedMeV) ;

		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		      //std::cout<<"Initial Kinetic Energy of Nitrogen :"<<EnergyMeV<<std::endl;
		      //std::cout<<"Total Energy Deposited deposited by Nitrogen :"<<EnergyDepositedMeV<<std::endl;

		    }
		}
	      
	       
	      if(PDG==1000080160 || PDG==1000080170 || PDG==1000080180)   //Oxygen
		 
		{
		  if (TrackID != prev_TrackID || EventID != prev_EventID)
		     
		    {
		
		      DoseDepositedOxygenInner = DoseDepositedOxygenInner + EnergyDepositedMeV;
		      PreStepKEOxygenInner = PreStepKEOxygenInner + EnergyMeV;
		      PostStepKEOxygenInner = PostStepKEOxygenInner + (EnergyMeV -EnergyDepositedMeV) ;
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		      //std::cout<<"Initial Kinetic Energy of Oxygen :"<<EnergyMeV<<std::endl;
		      //std::cout<<"Total Energy Deposited deposited by Oxygen :"<<EnergyDepositedMeV<<std::endl;

		    }
		}
	       
	      	       
	      if(PDG==11)   //Electrons
		 
		{
		  if (TrackID != prev_TrackID || EventID != prev_EventID)
		     
		    {
		       
		      DoseDepositedElectronInner = DoseDepositedElectronInner + EnergyDepositedMeV;
		      PreStepKEElectronInner = PreStepKEElectronInner + EnergyMeV;
		      PostStepKEElectronInner = PostStepKEElectronInner + (EnergyMeV -EnergyDepositedMeV) ;

		      
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		      //std::cout<<"Initial Kinetic Energy of Electrons :"<<EnergyMeV<<std::endl;
		      //std::cout<<"Total Energy Deposited deposited by Electrons :"<<EnergyDepositedMeV<<std::endl;

		    }
		}

	      if(PDG==-11)   //Positrons
		 
		{
		  if (TrackID != prev_TrackID || EventID != prev_EventID)
		     
		    {
		       
		      DoseDepositedPositronInner = DoseDepositedPositronInner + EnergyDepositedMeV;
		      PreStepKEPositronInner = PreStepKEPositronInner + EnergyMeV;
		      PostStepKEPositronInner = PostStepKEPositronInner + (EnergyMeV -EnergyDepositedMeV) ;

		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		      //std::cout<<"Initial Kinetic Energy of Positrons :"<<EnergyMeV<<std::endl;
		      //std::cout<<"Total Energy Deposited deposited by Positrons :"<<EnergyDepositedMeV<<std::endl;

		    }
		}
	      
	    }
	}
    }

       
  std::cout<<"Total PreStep KE of Protons in inner scorer :"<<PreStepKEProtonInner<<std::endl;
  std::cout<<"Total PostStep KE of Protons in inner scorer :"<<PostStepKEProtonInner<<std::endl;
  std::cout<<"Total dose deposited by Protons in Inner scorer :"<<DoseDepositedProtonInner<<std::endl;
  
  std::cout<<"Total PreStep KE of Deuterons in inner scorer :"<<PreStepKEDeuteronInner<<std::endl;
  std::cout<<"Total PostStep KE of Deuterons in inner scorer :"<<PostStepKEDeuteronInner<<std::endl;
  std::cout<<"Total dose deposited by Deuterons in Inner scorer :"<<DoseDepositedDeuteronInner<<std::endl;

  std::cout<<"Total PreStep KE of Tritons in inner scorer :"<<PreStepKETritonInner<<std::endl;
  std::cout<<"Total PostStep KE of Tritons in inner scorer :"<<PostStepKETritonInner<<std::endl;
  std::cout<<"Total dose deposited by Tritons in Inner scorer :"<<DoseDepositedTritonInner<<std::endl;

  std::cout<<"Total PreStep KE of Alphas in inner scorer :"<<PreStepKEAlphaInner<<std::endl;
  std::cout<<"Total PostStep KE of Alphas in inner scorer :"<<PostStepKEAlphaInner<<std::endl;
  std::cout<<"Total dose deposited by Alphas in Inner scorer :"<<DoseDepositedAlphaInner<<std::endl;

  std::cout<<"Total PreStep KE of Berylliums in inner scorer :"<<PreStepKEBerylliumInner<<std::endl;
  std::cout<<"Total PostStep KE of Berylliums in inner scorer :"<<PostStepKEBerylliumInner<<std::endl;
  std::cout<<"Total dose deposited by Berylliums in Inner scorer :"<<DoseDepositedBerylliumInner<<std::endl;
 
  std::cout<<"Total PreStep KE of Borons in inner scorer :"<<PreStepKEBoronInner<<std::endl;
  std::cout<<"Total PostStep KE of Borons in inner scorer :"<<PostStepKEBoronInner<<std::endl;
  std::cout<<"Total dose deposited by Borons in Inner scorer :"<<DoseDepositedBoronInner<<std::endl;
 
  std::cout<<"Total PreStep KE of Carbons in inner scorer :"<<PreStepKECarbonInner<<std::endl;
  std::cout<<"Total PostStep KE of Carbons in inner scorer :"<<PostStepKECarbonInner<<std::endl;
  std::cout<<"Total dose deposited by Carbons in Inner scorer :"<<DoseDepositedCarbonInner<<std::endl;
 
  std::cout<<"Total PreStep KE of Nitrogens in inner scorer :"<<PreStepKENitrogenInner<<std::endl;
  std::cout<<"Total PostStep KE of Nitrogens in inner scorer :"<<PostStepKENitrogenInner<<std::endl;
  std::cout<<"Total dose deposited by Nitrogens in Inner scorer :"<<DoseDepositedNitrogenInner<<std::endl;

  std::cout<<"Total PreStep KE of Oxygens in inner scorer :"<<PreStepKEOxygenInner<<std::endl;
  std::cout<<"Total PostStep KE of Oxygens in inner scorer :"<<PostStepKEOxygenInner<<std::endl;
  std::cout<<"Total dose deposited by Oxygens in Inner scorer :"<<DoseDepositedOxygenInner<<std::endl;

  std::cout<<"Total PreStep KE of Electrons in inner scorer :"<<PreStepKEElectronInner<<std::endl;
  std::cout<<"Total PostStep KE of Electrons in inner scorer :"<<PostStepKEElectronInner<<std::endl;
  std::cout<<"Total dose deposited by Electrons in Inner scorer :"<<DoseDepositedElectronInner<<std::endl;
 
  std::cout<<"Total PreStep KE of Positrons in inner scorer :"<<PreStepKEPositronInner<<std::endl;
  std::cout<<"Total PostStep KE of Positrons in inner scorer :"<<PostStepKEPositronInner<<std::endl;
  std::cout<<"Total dose deposited by Positrons in Inner scorer :"<<DoseDepositedPositronInner<<std::endl;
 
  TotalDoseDepositedInner = DoseDepositedProtonInner + DoseDepositedDeuteronInner + DoseDepositedTritonInner + DoseDepositedAlphaInner + DoseDepositedBerylliumInner + DoseDepositedBoronInner + DoseDepositedCarbonInner + DoseDepositedNitrogenInner + DoseDepositedOxygenInner + DoseDepositedElectronInner + DoseDepositedPositronInner ;

  TotalPreStepKEInner = PreStepKEProtonInner + PreStepKEDeuteronInner + PreStepKETritonInner + PreStepKEAlphaInner + PreStepKEBerylliumInner + PreStepKEBoronInner + PreStepKECarbonInner + PreStepKENitrogenInner + PreStepKEOxygenInner + PreStepKEElectronInner + PreStepKEPositronInner ; 

  TotalPostStepKEInner = PostStepKEProtonInner + PostStepKEDeuteronInner + PostStepKETritonInner + PostStepKEAlphaInner + PostStepKEBerylliumInner + PostStepKEBoronInner + PostStepKECarbonInner + PostStepKENitrogenInner + PostStepKEOxygenInner + PostStepKEElectronInner + PostStepKEPositronInner ; 


  std::cout<< "Total dose deposited  in Inner scorer: "<< TotalDoseDepositedInner<<" MeV"<<std::endl;
  std::cout<< "Total PreStep KE deposited  in Inner scorer: "<< TotalPreStepKEInner<<" MeV"<<std::endl;
  std::cout<< "Total PostStep KE deposited  in Inner scorer: "<< TotalPostStepKEInner<<" MeV"<<std::endl;


  std::cout<< "Relative dose contribution by Protons in Inner scorer: "<<DoseDepositedProtonInner/TotalDoseDepositedInner<<std::endl;
  std::cout<< "Relative dose contribution by Deuterons in Inner scorer: "<<DoseDepositedDeuteronInner/TotalDoseDepositedInner<<std::endl;
  std::cout<< "Relative dose contribution by Tritons in Inner scorer: "<<DoseDepositedTritonInner/TotalDoseDepositedInner<<std::endl;
  std::cout<< "Relative dose contribution by Alphas in Inner scorer: "<<DoseDepositedAlphaInner/TotalDoseDepositedInner<<std::endl;
  std::cout<< "Relative dose contribution by Berylliums in Inner scorer: "<<DoseDepositedBerylliumInner/TotalDoseDepositedInner<<std::endl;
  std::cout<< "Relative dose contribution by Borons in Inner scorer: "<<DoseDepositedBoronInner/TotalDoseDepositedInner<<std::endl;
  std::cout<< "Relative dose contribution by Carbons in Inner scorer: "<<DoseDepositedCarbonInner/TotalDoseDepositedInner<<std::endl;
  std::cout<< "Relative dose contribution by Nitrogen in Inner scorer: "<<DoseDepositedNitrogenInner/TotalDoseDepositedInner<<std::endl;
  std::cout<< "Relative dose contribution by Oxygen in Inner scorer: "<<DoseDepositedOxygenInner/TotalDoseDepositedInner<<std::endl;
  std::cout<< "Relative dose contribution by Electrons in Inner scorer: "<<DoseDepositedElectronInner/TotalDoseDepositedInner<<std::endl;
  std::cout<< "Relative dose contribution by Positrons in Inner scorer: "<<DoseDepositedPositronInner/TotalDoseDepositedInner<<std::endl;

  
  std::cout<< "Relative dose contribution by Secondary Particles in Inner scorer: "<<DoseDepositedProtonInner/TotalDoseDepositedInner<<","<<DoseDepositedDeuteronInner/TotalDoseDepositedInner<<","<<DoseDepositedTritonInner/TotalDoseDepositedInner<<","<<DoseDepositedAlphaInner/TotalDoseDepositedInner<<","<<DoseDepositedBerylliumInner/TotalDoseDepositedInner<<","<<DoseDepositedBoronInner/TotalDoseDepositedInner<<","<<DoseDepositedCarbonInner/TotalDoseDepositedInner<<","<<DoseDepositedNitrogenInner/TotalDoseDepositedInner<<","<<DoseDepositedOxygenInner/TotalDoseDepositedInner<<","<<DoseDepositedElectronInner/TotalDoseDepositedInner<<","<<DoseDepositedPositronInner/TotalDoseDepositedInner<<std::endl;


  double DoseDepositedProtonInter = 0;
  double DoseDepositedDeuteronInter = 0;
  double DoseDepositedTritonInter = 0;
  double DoseDepositedAlphaInter = 0;
  double DoseDepositedBerylliumInter = 0;
  double DoseDepositedBoronInter = 0;
  double DoseDepositedCarbonInter = 0;
  double DoseDepositedNitrogenInter = 0;
  double DoseDepositedOxygenInter = 0;
  double DoseDepositedElectronInter = 0;
  double DoseDepositedPositronInter = 0;
 

  // std::ofstream out("proton_dose_inner.txt");

   
  if (treeInter)
    {

      if (!f || f->IsZombie())
	{
	  std::cerr << "Error: could not open file " << std::endl;
	  return 1;
	}
       
      if (f->IsOpen())
	{
	  for (int i = 0; i < treeInter->GetEntries(); i++)
	    {
	      treeInter->GetEntry(i);
	       
	      if(PDG==2212)  //Protons 

		{
		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {
		    	    
		      DoseDepositedProtonInter = DoseDepositedProtonInter + EnergyDepositedMeV;
		      //DoseDepositedProtonInter = DoseDepositedProtonInter + (EnergyMeV - EnergyDepositedMeV);
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		      //std::cout<<"EnergyDeposited deposited by Protons"<<EnergyDepositedMeV<<std::endl;
		      // out << EnergyDepositedMeV << '\n'; 
		   
		    }
		}

	      
	      
	      if(PDG==1000010020)  //Deuterons

		{
		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {
		      DoseDepositedDeuteronInter = DoseDepositedDeuteronInter + EnergyDepositedMeV;
		      //DoseDepositedDeuteronInter = DoseDepositedDeuteronInter + (EnergyMeV - EnergyDepositedMeV);
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<"EnergyDeposited deposited by Deuterons"<<EnergyDepositedMeV<<std::endl;
		   
		    }
		}

	      	      if(PDG==1000010030)  //Tritons

		{
		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {
		      DoseDepositedTritonInter = DoseDepositedTritonInter + EnergyDepositedMeV;
		      //DoseDepositedTritonInter = DoseDepositedTritonInter + (EnergyMeV - EnergyDepositedMeV);
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<"EnergyDeposited deposited by Tritons"<<EnergyDepositedMeV<<std::endl;
		   
		    }
		}

			       
	      if(PDG==1000020040)   //alphas
		 
		{

		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {

		      DoseDepositedAlphaInter = DoseDepositedAlphaInter + EnergyDepositedMeV;
		      //DoseDepositedAlphaInter = DoseDepositedAlphaInter + (EnergyMeV - EnergyDepositedMeV);
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		    }
		}

	          if(PDG==1000040090 || PDG==1000040100)   //Beryllium
	     
		{

		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {

		      DoseDepositedBerylliumInter = DoseDepositedBerylliumInter + EnergyDepositedMeV;
		      //DoseDepositedBerylliumInter = DoseDepositedBerylliumInter + (EnergyMeV - EnergyDepositedMeV);
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		    }
		}

		      if(PDG==1000050100 || PDG==1000050110)   //Boron
	     
		{

		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {

		      DoseDepositedBoronInter = DoseDepositedBoronInter + EnergyDepositedMeV;
		      //DoseDepositedBoronInter = DoseDepositedBoronInter + (EnergyMeV - EnergyDepositedMeV);
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		    }
		}

	       
	      if(PDG==1000060120 || PDG==1000060130 || PDG==1000060140 || PDG==1000060150)   //Carbon
	     
		{

		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {

		      DoseDepositedCarbonInter = DoseDepositedCarbonInter + EnergyDepositedMeV;
		      //DoseDepositedCarbonInter = DoseDepositedCarbonInter + (EnergyMeV - EnergyDepositedMeV);
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		    }
		}

  	       
	      if(PDG==1000070140 || PDG==1000070150 || PDG==1000070160)   //Nitrogen
		 
		{

		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {
		
		      DoseDepositedNitrogenInter = DoseDepositedNitrogenInter + EnergyDepositedMeV;
		      //DoseDepositedNitrogenInter = DoseDepositedNitrogenInter + (EnergyMeV - EnergyDepositedMeV);
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		    }
		}
	      
	       
	      if(PDG==1000080160 || PDG==1000080170 || PDG==1000080180)   //Oxygen
		 
		{
		  if (TrackID != prev_TrackID || EventID != prev_EventID)
		     
		    {
		
		      DoseDepositedOxygenInter = DoseDepositedOxygenInter + EnergyDepositedMeV;
		      //DoseDepositedOxygenInter = DoseDepositedOxygenInter + (EnergyMeV - EnergyDepositedMeV);
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		    }
		}
	       
	      	       
	      if(PDG==11)   //Electrons
		 
		{
		  if (TrackID != prev_TrackID || EventID != prev_EventID)
		     
		    {
		       
		      //DoseDepositedElectronInter = DoseDepositedElectronInter + EnergyMeV;
		      DoseDepositedElectronInter = DoseDepositedElectronInter + EnergyDepositedMeV;
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		    }
		}

	      if(PDG==-11)   //Positrons
		 
		{
		  if (TrackID != prev_TrackID || EventID != prev_EventID)
		     
		    {
		       
		      //DoseDepositedPositronInter = DoseDepositedPositronInter + EnergyMeV;
		      DoseDepositedPositronInter = DoseDepositedPositronInter + EnergyDepositedMeV;
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		    }
		}
	      
	    }
	}
    }

       


  TotalDoseDepositedInter = DoseDepositedProtonInter + DoseDepositedDeuteronInter + DoseDepositedTritonInter + DoseDepositedAlphaInter + DoseDepositedBerylliumInter + DoseDepositedBoronInter + DoseDepositedCarbonInter + DoseDepositedNitrogenInter + DoseDepositedOxygenInter + DoseDepositedElectronInter + DoseDepositedPositronInter ; 
    
  std::cout<< "Total dose deposited  in Inter scorer: "<< TotalDoseDepositedInter<<" MeV"<<std::endl;
  std::cout<< "Relative dose contribution by Protons in Inter scorer: "<<DoseDepositedProtonInter/TotalDoseDepositedInter<<std::endl;
  std::cout<< "Relative dose contribution by Deuterons in Inter scorer: "<<DoseDepositedDeuteronInter/TotalDoseDepositedInter<<std::endl;
  std::cout<< "Relative dose contribution by Tritons in Inter scorer: "<<DoseDepositedTritonInter/TotalDoseDepositedInter<<std::endl;
  std::cout<< "Relative dose contribution by Alphas in Inter scorer: "<<DoseDepositedAlphaInter/TotalDoseDepositedInter<<std::endl;
  std::cout<< "Relative dose contribution by Berylliums in Inter scorer: "<<DoseDepositedBerylliumInter/TotalDoseDepositedInter<<std::endl;
  std::cout<< "Relative dose contribution by Borons in Inter scorer: "<<DoseDepositedBoronInter/TotalDoseDepositedInter<<std::endl;
  std::cout<< "Relative dose contribution by Carbons in Inter scorer: "<<DoseDepositedCarbonInter/TotalDoseDepositedInter<<std::endl;
  std::cout<< "Relative dose contribution by Nitrogen in Inter scorer: "<<DoseDepositedNitrogenInter/TotalDoseDepositedInter<<std::endl;
  std::cout<< "Relative dose contribution by Oxygen in Inter scorer: "<<DoseDepositedOxygenInter/TotalDoseDepositedInter<<std::endl;
  std::cout<< "Relative dose contribution by Electrons in Inter scorer: "<<DoseDepositedElectronInter/TotalDoseDepositedInter<<std::endl;
  std::cout<< "Relative dose contribution by Positrons in Inter scorer: "<<DoseDepositedPositronInter/TotalDoseDepositedInter<<std::endl;

  
  std::cout<< "Relative dose contribution by Secondary Particles in Inter scorer: "<<DoseDepositedProtonInter/TotalDoseDepositedInter<<","<<DoseDepositedDeuteronInter/TotalDoseDepositedInter<<","<<DoseDepositedTritonInter/TotalDoseDepositedInter<<","<<DoseDepositedAlphaInter/TotalDoseDepositedInter<<","<<DoseDepositedBerylliumInter/TotalDoseDepositedInter<<","<<DoseDepositedBoronInter/TotalDoseDepositedInter<<","<<DoseDepositedCarbonInter/TotalDoseDepositedInter<<","<<DoseDepositedNitrogenInter/TotalDoseDepositedInter<<","<<DoseDepositedOxygenInter/TotalDoseDepositedInter<<","<<DoseDepositedElectronInter/TotalDoseDepositedInter<<","<<DoseDepositedPositronInter/TotalDoseDepositedInter<<std::endl;


  double DoseDepositedProtonOuter = 0;
  double DoseDepositedDeuteronOuter = 0;
  double DoseDepositedTritonOuter = 0;
  double DoseDepositedAlphaOuter = 0;
  double DoseDepositedBerylliumOuter = 0;
  double DoseDepositedBoronOuter = 0;
  double DoseDepositedCarbonOuter = 0;
  double DoseDepositedNitrogenOuter = 0;
  double DoseDepositedOxygenOuter = 0;
  double DoseDepositedElectronOuter = 0;
  double DoseDepositedPositronOuter = 0;
 

  // std::ofstream out("proton_dose_inner.txt");

   
  if (treeOuter)
    {

      if (!f || f->IsZombie())
	{
	  std::cerr << "Error: could not open file " << std::endl;
	  return 1;
	}
       
      if (f->IsOpen())
	{
	  for (int i = 0; i < treeOuter->GetEntries(); i++)
	    {
	      treeOuter->GetEntry(i);
	       
	      if(PDG==2212)  //Protons 

		{
		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {
		    	    
		      DoseDepositedProtonOuter = DoseDepositedProtonOuter + EnergyDepositedMeV;
		      //DoseDepositedProtonOuter = DoseDepositedProtonOuter + (EnergyMeV - EnergyDepositedMeV);
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		       //std::cout<<DoseMeV<<" has EventID "<<EventID<<std::endl;
		       //std::cout<<"DoseDeposited deposited by Protons"<<DoseDepositedMeV<<std::endl;
		      //out << DoseDepositedMeV << '\n'; 
		   
		    }
		}

	      
	      
	      if(PDG==1000010020)  //Deuterons

		{
		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {
		      DoseDepositedDeuteronOuter = DoseDepositedDeuteronOuter + EnergyDepositedMeV;
		      //DoseDepositedDeuteronOuter = DoseDepositedDeuteronOuter + (EnergyMeV - EnergyDepositedMeV);
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<"EnergyDeposited deposited by Deuterons"<<EnergyDepositedMeV<<std::endl;
		   
		    }
		}

	      	      if(PDG==1000010030)  //Tritons

		{
		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {
		      DoseDepositedTritonOuter = DoseDepositedTritonOuter + EnergyDepositedMeV;
		      //DoseDepositedTritonOuter = DoseDepositedTritonOuter + (EnergyMeV - EnergyDepositedMeV);
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<"EnergyDeposited deposited by Tritons"<<EnergyDepositedMeV<<std::endl;
		   
		    }
		}

			       
	      if(PDG==1000020040)   //alphas
		 
		{

		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {

		      DoseDepositedAlphaOuter = DoseDepositedAlphaOuter + EnergyDepositedMeV;
		      //DoseDepositedAlphaOuter = DoseDepositedAlphaOuter + (EnergyMeV - EnergyDepositedMeV);
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		    }
		}

	          if(PDG==1000040090 || PDG==1000040100)   //Beryllium
	     
		{

		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {

		      DoseDepositedBerylliumOuter = DoseDepositedBerylliumOuter + EnergyDepositedMeV;
		      //DoseDepositedBerylliumOuter = DoseDepositedBerylliumOuter + (EnergyMeV - EnergyDepositedMeV);
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		    }
		}

		      if(PDG==1000050100 || PDG==1000050110)   //Boron
	     
		{

		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {

		      DoseDepositedBoronOuter = DoseDepositedBoronOuter + EnergyDepositedMeV;
		      //DoseDepositedBoronOuter = DoseDepositedBoronOuter + (EnergyMeV - EnergyDepositedMeV);
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		    }
		}

	       
	      if(PDG==1000060120 || PDG==1000060130 || PDG==1000060140 || PDG==1000060150)   //Carbon
	     
		{

		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {

		      DoseDepositedCarbonOuter = DoseDepositedCarbonOuter + EnergyDepositedMeV;
		      //DoseDepositedCarbonOuter = DoseDepositedCarbonOuter + (EnergyMeV - EnergyDepositedMeV);
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		    }
		}

  	       
	      if(PDG==1000070140 || PDG==1000070150 || PDG==1000070160)   //Nitrogen
		 
		{

		  if (TrackID != prev_TrackID || EventID != prev_EventID)

		    {
		
		      DoseDepositedNitrogenOuter = DoseDepositedNitrogenOuter + EnergyDepositedMeV;
		      //DoseDepositedNitrogenOuter = DoseDepositedNitrogenOuter + (EnergyMeV - EnergyDepositedMeV);
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		    }
		}
	      
	       
	      if(PDG==1000080160 || PDG==1000080170 || PDG==1000080180)   //Oxygen
		 
		{
		  if (TrackID != prev_TrackID || EventID != prev_EventID)
		     
		    {
		
		      DoseDepositedOxygenOuter = DoseDepositedOxygenOuter + EnergyDepositedMeV;
		      //DoseDepositedOxygenOuter = DoseDepositedOxygenOuter + (EnergyMeV - EnergyDepositedMeV);
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		    }
		}
	       
	      	       
	      if(PDG==11)   //Electrons
		 
		{
		  if (TrackID != prev_TrackID || EventID != prev_EventID)
		     
		    {
		       
		      //DoseDepositedElectronOuter = DoseDepositedElectronOuter + EnergyMeV;
		      DoseDepositedElectronOuter = DoseDepositedElectronOuter + EnergyDepositedMeV;
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		    }
		}

	      if(PDG==-11)   //Positrons
		 
		{
		  if (TrackID != prev_TrackID || EventID != prev_EventID)
		     
		    {
		       
		      //DoseDepositedPositronOuter = DoseDepositedPositronOuter + EnergyMeV;
		      DoseDepositedPositronOuter = DoseDepositedPositronOuter + EnergyDepositedMeV;
		      prev_TrackID = TrackID;
		      prev_EventID = EventID;
		      //std::cout<<EnergyMeV<<" has EventID "<<EventID<<std::endl;
		    }
		}
	      
	    }
	}
    }

       

  TotalDoseDepositedOuter = DoseDepositedProtonOuter + DoseDepositedDeuteronOuter + DoseDepositedTritonOuter + DoseDepositedAlphaOuter + DoseDepositedBerylliumOuter + DoseDepositedBoronOuter + DoseDepositedCarbonOuter + DoseDepositedNitrogenOuter + DoseDepositedOxygenOuter + DoseDepositedElectronOuter + DoseDepositedPositronOuter ; 
    
  std::cout<< "Total dose deposited  in Outer scorer: "<< TotalDoseDepositedOuter<<" MeV"<<std::endl;
  std::cout<< "Relative dose contribution by Protons in Outer scorer: "<<DoseDepositedProtonOuter/TotalDoseDepositedOuter<<std::endl;
  std::cout<< "Relative dose contribution by Deuterons in Outer scorer: "<<DoseDepositedDeuteronOuter/TotalDoseDepositedOuter<<std::endl;
  std::cout<< "Relative dose contribution by Tritons in Outer scorer: "<<DoseDepositedTritonOuter/TotalDoseDepositedOuter<<std::endl;
  std::cout<< "Relative dose contribution by Alphas in Outer scorer: "<<DoseDepositedAlphaOuter/TotalDoseDepositedOuter<<std::endl;
  std::cout<< "Relative dose contribution by Berylliums in Outer scorer: "<<DoseDepositedBerylliumOuter/TotalDoseDepositedOuter<<std::endl;
  std::cout<< "Relative dose contribution by Borons in Outer scorer: "<<DoseDepositedBoronOuter/TotalDoseDepositedOuter<<std::endl;
  std::cout<< "Relative dose contribution by Carbons in Outer scorer: "<<DoseDepositedCarbonOuter/TotalDoseDepositedOuter<<std::endl;
  std::cout<< "Relative dose contribution by Nitrogen in Outer scorer: "<<DoseDepositedNitrogenOuter/TotalDoseDepositedOuter<<std::endl;
  std::cout<< "Relative dose contribution by Oxygen in Outer scorer: "<<DoseDepositedOxygenOuter/TotalDoseDepositedOuter<<std::endl;
  std::cout<< "Relative dose contribution by Electrons in Outer scorer: "<<DoseDepositedElectronOuter/TotalDoseDepositedOuter<<std::endl;
  std::cout<< "Relative dose contribution by Positrons in Outer scorer: "<<DoseDepositedPositronOuter/TotalDoseDepositedOuter<<std::endl;

  
  std::cout<< "Relative dose contribution by Secondary Particles in Outer scorer: "<<DoseDepositedProtonOuter/TotalDoseDepositedOuter<<","<<DoseDepositedDeuteronOuter/TotalDoseDepositedOuter<<","<<DoseDepositedTritonOuter/TotalDoseDepositedOuter<<","<<DoseDepositedAlphaOuter/TotalDoseDepositedOuter<<","<<DoseDepositedBerylliumOuter/TotalDoseDepositedOuter<<","<<DoseDepositedBoronOuter/TotalDoseDepositedOuter<<","<<DoseDepositedCarbonOuter/TotalDoseDepositedOuter<<","<<DoseDepositedNitrogenOuter/TotalDoseDepositedOuter<<","<<DoseDepositedOxygenOuter/TotalDoseDepositedOuter<<","<<DoseDepositedElectronOuter/TotalDoseDepositedOuter<<","<<DoseDepositedPositronOuter/TotalDoseDepositedOuter<<std::endl;




  return 0;
}

    

