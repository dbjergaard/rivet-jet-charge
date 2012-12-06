#include "Pythia.h"
#include "HepMCInterface.h"

#include "HepMC/GenEvent.h"   
#include "HepMC/IO_GenEvent.h"

#ifdef HEPMC_HAS_UNITS
#include "HepMC/Units.h"
#endif
using namespace Pythia8; 

int main(int argc, char* argv[]) 
{
  if(argc !=3)
    {
      cerr<<"Usage: "<<argv[0]<<" [config file] [out file]"<<endl;
      return 1;
    }
  ifstream is(argv[1]);  
  if (!is) {
    cerr << " Command-line file " << argv[1] << " was not found. \n"
         << " Program stopped! " << endl;
    return 1;
  }
  cout<<"Configuring PYTHIA from "<<argv[1]<<endl;
  cout<<"Writing output to "<<argv[2]<<endl;

  HepMC::I_Pythia8 ToHepMC;
  HepMC::IO_GenEvent ascii_io(argv[2], std::ios::out);

  Pythia pythia;
  pythia.readFile(argv[1]);
  int    nEvent    = 100;
  nEvent = pythia.mode("Main:numberOfEvents");
  int    nAbort    = pythia.mode("Main:timesAllowErrors");
  pythia.init();
  
  int iAbort = 0;
  for(int iEvent =0; iEvent < nEvent; ++iEvent)
    {
      if (!pythia.next()) 
	{
	  if (pythia.info.atEndOfFile()) 
	    {
	      cout << " Aborted since reached end of Les Houches Event File\n"; 
	      break; 
	    }
	  
	  if (++iAbort < nAbort) continue;
	  cout << " Event generation aborted prematurely, owing to error!\n"; 
	  break;
	}
      ToHepMC.fill_next_event( pythia, hepmcevt );
      HepMC::GenEvent* hepmcevt = new HepMC::GenEvent(HepMC::Units::GEV, HepMC::Units::MM);
      ascii_io << hepmcevt;
      delete hepmcevt;
    }

  pythia.stat();
  return 0;
}
