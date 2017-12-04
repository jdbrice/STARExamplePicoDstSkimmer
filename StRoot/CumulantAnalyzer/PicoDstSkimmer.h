#ifndef PICO_DST_SKIMMER_H
#define PICO_DST_SKIMMER_H

#include <string>

// StRoot
#include "StMaker.h"
#include "StThreeVectorF.hh"
#include "StThreeVectorD.hh"


// ROOT
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"


class StPicoDstMaker;
class StPicoDst;
class StPicoEvent;
class StPicoTrack;

class PicoDstSkimmer : public StMaker 
{
public:
	PicoDstSkimmer( const Char_t *name = "PicoDstSkimmer" );
	~PicoDstSkimmer();
	
	Int_t Init();
	Int_t Make();
	Int_t Finish();

	void setOutputFileName( std::string fn){
		this->_outputFilename = fn;
	}

protected:

	std::string _outputFilename;

	TFile *_rootFile;

	StPicoDst          *_StPicoDst;
	StPicoEvent        *_StPicoEvent;

	void analyzeTrack( StPicoTrack * picoTrack );

	ClassDef(PicoDstSkimmer, 1)
};


#endif