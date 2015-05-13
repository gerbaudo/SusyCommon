
#include <cstdlib>
#include <string>

#include "TChain.h"
#include "TSystem.h"

#include "SusyCommon/SusyNtMaker.h"
#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/ChainHelper.h"
#include "SusyNtuple/string_utils.h"


using namespace std;
using namespace Susy;
using Susy::SusyNtMaker;

/*

    SusyNtMaker - an executable for running a basic looper/dumper of SUSY D3PDs
    It is not intended to be very advanced but more to look quickly at a d3pd and test the code

*/

void help()
{
  cout << "  Options:"                          << endl;
  cout << "  -n number of events to process"    << endl;
  cout << "     defaults: -1 (all events)"      << endl;

  cout << "  -k number of events to skip"       << endl;
  cout << "     defaults: 0"                    << endl;

  cout << "  -d debug printout level"           << endl;
  cout << "     defaults: 0 (quiet) "           << endl;

  cout << "  -f name of input filelist"         << endl;
  cout << "     defaults: fileList.txt"         << endl;

  cout << "  -s sample name, sets isMC flag"    << endl;
  cout << "     use e.g. 'ttbar', 'DataG', etc" << endl;

  cout << "  -w set sum of mc weights for norm" << endl;
  cout << "     default: 1"                     << endl;

  cout << "  -x set cross section"              << endl;
  cout << "     default: -1 (use susy db)"      << endl;

  cout << "  -l set lumi"                       << endl;
  cout << "     default: 5312/pb"               << endl;

  cout << "  -m write output ntuple"            << endl;
  cout << "     default: 1 (true)"              << endl;

  cout << "  --grl specify GRL"                 << endl;
  cout << "     default: set in cxx"            << endl;

  cout << "  --sys will turn on systematic run" << endl;
  cout << "     default: off"                   << endl;

  cout << "  --savePh will save photons"        << endl;
  cout << "     default: off"                   << endl;

  cout << "  --saveTau will save taus"          << endl;
  cout << "     default: off"                   << endl;

  cout << "  --saveContTau will save container" << endl;
  cout << "     taus instead of selected"       << endl;

  cout << "  --saveTruth will save truth"       << endl;
  cout << "     default: off"                   << endl;

  cout << "  --af2 specifies AF2 samples"       << endl;
  cout << "     default: off"                   << endl;

  cout << "  --d3pd1032 sets d3pd tag to p1032" << endl;
  cout << "     default: p1181"                 << endl;

  cout << "  --metFlav set met flavor"          << endl;
  cout << "     default: Default"               << endl;

  //cout << "  --useMetMuons set to use met muons"<< endl;
  //cout << "     for calculating missing energy "<< endl;
  //cout << "     default: off"                   << endl;

  //cout << "  --doMetFix turn on MET ele-jet"    << endl;
  //cout << "     overlap fix"                    << endl;
  //cout << "     default: off"                   << endl;

  cout << "  --filterOff turns off filtering"   << endl;
  cout << "     default: filter is on"          << endl;

  cout << "  --nLepFilter number of light leps" << endl;
  cout << "     to filter on. Default: 0"       << endl;

  cout << "  --nLepTauFilter number of total"   << endl;
  cout << "     light+tau to filter on."        << endl;
  cout << "     Default: 2"                     << endl;

  cout << "  --filterTrig turns on trigger"     << endl;
  cout << "     filtering."                     << endl;

  cout << "  --trig set which triggers are  "   << endl;
  cout << "    stored.                      "   << endl;
  cout << "    Default: 1 (0: Run1, 1:Run2) "   << endl;
  cout << "--input  name of the input container" <<endl;
  cout << "--output name of the output container"<<endl;
  cout << "--tag    SustNtuple production tag"   <<endl;
  cout << "  -h print this help"                << endl;
}


int main(int argc, char** argv)
{
  int nEvt        = -1;
  int nSkip       = 0;
  int dbg         = 0;
  float lumi      = 5831;
  string sample   = "";
  string fileList = "fileList.txt";
  string grl      = "";
  bool sysOn      = false;
  bool savePh     = false;
  bool saveTau    = false;
  bool saveContTau= false;
  bool saveTruth  = false;
  bool isAF2      = false;
  bool writeNt    = true;
  D3PDTag tag     = D3PD_p1328;
  string metFlav  = "Default";
  //bool doMetFix   = false;
  bool filter     = true;
  uint nLepFilter = 0;
  uint nLepTauFilter = 2;
  bool filterTrig = false;
  string trigset     = "run2";
  string inputContainer, outputContainer, ntTag;

  cout << "SusyNtMaker" << endl;
  cout << endl;

  // Read inputs to program
  for(int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-n") == 0)
      nEvt = atoi(argv[++i]);
    else if (strcmp(argv[i], "-k") == 0)
      nSkip = atoi(argv[++i]);
    else if (strcmp(argv[i], "-d") == 0)
      dbg = atoi(argv[++i]);
    else if (strcmp(argv[i], "-f") == 0)
      fileList = argv[++i];
    else if (strcmp(argv[i], "-s") == 0)
      sample = argv[++i];
    else if (strcmp(argv[i], "-l") == 0)
      lumi = atof(argv[++i]);
    else if (strcmp(argv[i], "-m") == 0)
      writeNt = atoi(argv[++i]);
    else if (strcmp(argv[i], "--grl") == 0)
      grl = argv[++i];
    else if (strcmp(argv[i], "--sys") == 0)
      sysOn = true;
    else if (strcmp(argv[i], "--savePh") == 0)
    savePh = true;
    else if (strcmp(argv[i], "--saveTau") == 0)
      saveTau = true;
    else if (strcmp(argv[i], "--saveContTau") == 0){
      saveTau = true;
      saveContTau = true;
    }
    else if (strcmp(argv[i], "--saveTruth") == 0)
      saveTruth = true;
    else if (strcmp(argv[i], "--af2") == 0)
      isAF2 = true;
    else if (strcmp(argv[i], "--d3pd1032") == 0)
      tag = D3PD_p1032;
    else if (strcmp(argv[i], "--metFlav") == 0)
      metFlav = argv[++i];
    //else if (strcmp(argv[i], "--doMetFix") == 0)
      //doMetFix = true;
    else if (strcmp(argv[i], "--filterOff") == 0)
      filter = false;
    else if (strcmp(argv[i], "--nLepFilter") == 0)
      nLepFilter = atoi(argv[++i]);
    else if (strcmp(argv[i], "--nLepTauFilter") == 0)
      nLepTauFilter = atoi(argv[++i]);
    else if (strcmp(argv[i], "--filterTrig") == 0)
      filterTrig = true;
    else if (strcmp(argv[i], "--triggerSet") == 0)
      trigset = argv[++i];
    else if (strcmp(argv[i], "--input") == 0) inputContainer = argv[++i];
    else if (strcmp(argv[i], "--output") == 0) outputContainer = argv[++i];
    else if (strcmp(argv[i], "--tag") == 0) ntTag = argv[++i];
    else
    {
      help();
      return 0;
    }
  }

  cout << "flags:" << endl;
  cout << "  sample        " << sample   << endl;
  cout << "  nEvt          " << nEvt     << endl;
  cout << "  nSkip         " << nSkip    << endl;
  cout << "  dbg           " << dbg      << endl;
  cout << "  fileList      " << fileList << endl;
  cout << "  grl           " << grl      << endl;
  cout << "  sys           " << sysOn    << endl;
  cout << "  savePh        " << savePh   << endl;
  cout << "  saveTau       " << saveTau  << endl;
  cout << "  saveContTau   " << saveContTau << endl;
  cout << "  saveTru       " << saveTruth<< endl;
  cout << "  isAF2         " << isAF2    << endl;
  cout << "  d3pdtag       " << tag      << endl;
  cout << "  metFlav       " << metFlav  << endl;
  //cout << "  doMetFix      " << doMetFix << endl;
  cout << "  lumi          " << lumi     << endl;
  cout << "  filter        " << filter   << endl;
  cout << "  nLepFilter    " << nLepFilter    << endl;
  cout << "  nLepTauFilter " << nLepTauFilter << endl;
  cout << "  filterTrig    " << filterTrig    << endl;
  cout << "  triggerSet    " << trigset       << endl;
  cout << "  input         " << inputContainer  << endl;
  cout << "  output        " << outputContainer << endl;
  cout << "  ntTag         " << ntTag           << endl;
  cout << endl;


  // Build the input chain
  TChain* chain = new TChain("CollectionTree");
  int fileErr = ChainHelper::addInput(chain, fileList, dbg);
  if(fileErr) return 1;
  Long64_t nEntries = chain->GetEntries();
  chain->ls();

  // Build the TSelector
  SusyNtMaker* susyAna = new SusyNtMaker();
  susyAna->setDebug(dbg);
  susyAna->setTriggerSet(trigset);
  susyAna->setSample(sample);
  susyAna->setLumi(lumi);
  susyAna->setSys(sysOn);
  susyAna->setSelectPhotons(savePh);
  susyAna->setSelectTaus(saveTau);
  susyAna->setSaveContTaus(saveContTau);
  susyAna->setAF2(isAF2);
  susyAna->setFillNt(writeNt);
  susyAna->setD3PDTag(tag);
  susyAna->setMetFlavor(metFlav);
  susyAna->setSelectTruthObjects(saveTruth);
  //susyAna->setDoMetFix(doMetFix);
  susyAna->setFilter(filter);
  susyAna->setNLepFilter(nLepFilter);
  susyAna->setNLepTauFilter(nLepTauFilter);
  susyAna->setFilterTrigger(filterTrig);
  susyAna->m_inputContainerName = inputContainer;
  susyAna->m_outputContainerName = outputContainer;
  susyAna->m_productionTag = ntTag;
  susyAna->m_productionCommand = Susy::utils::commandLineArguments(argc, argv);
  // GRL - default is set in SusyD3PDAna::Begin, but now we can override it here
  susyAna->setGRLFile(grl);

  // Run the job
  if(nEvt<0) nEvt = nEntries;
  cout << endl;
  cout << "Total entries:   " << nEntries << endl;
  cout << "Process entries: " << nEvt << endl;
  chain->Process(susyAna, sample.c_str(), nEvt, nSkip);

  cout << endl;
  cout << "SusyNtMaker job done" << endl;

  delete chain;
  return 0;
}
