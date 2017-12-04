
// StRoot
#include "StThreeVectorF.hh"
#include "StarClassLibrary/StParticleDefinition.hh"

// StPicoDstMaker
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoDstMaker/StPicoDst.h"
#include "StPicoEvent/StPicoEvent.h"
#include "StPicoEvent/StPicoTrack.h"
#include "StPicoEvent/StPicoMtdPidTraits.h"
#include "StPicoEvent/StPicoMtdHit.h"

// STL
#include <vector>
#include <map>
#include <algorithm>


#include "CumulantAnalyzer/PicoDstSkimmer.h"

ClassImp(PicoDstSkimmer)



/* PicoDstSkimmer
 * Subclass of StMaker, see: http://www.star.bnl.gov/webdata/dox/html/classStMaker.html
 */
PicoDstSkimmer::PicoDstSkimmer( const Char_t *name ) : StMaker( name )
{
	this->_outputFilename = "output.root";
	SetDebug(1);
}

PicoDstSkimmer::~PicoDstSkimmer()
{

}



/* Init
 * Called once when chain starts running (triggered by Chain::Init() )
 */
Int_t PicoDstSkimmer::Init()
{
	// logger, lines must end with "endm" not "endl"
	LOG_INFO << "INITIALIZE" << endm;
	
	this->_rootFile = new TFile( this->_outputFilename.c_str(), "RECREATE" );
	this->_rootFile->cd();
	
	return kStOK;
}


/* Finish
 * Called once when chain finishes  ( triggered by Chain::Finish() )
 */
Int_t PicoDstSkimmer::Finish()
{
	LOG_INFO << "FINISH" <<endm;

	this->_rootFile->Write();
	this->_rootFile->Close();
	return kStOK;
}


/* Make
 * Called once per event (triggered by Chain::Make() )
 *
 */
Int_t PicoDstSkimmer::Make()
{
	LOG_INFO << "PicoDstSkimmer::Make()" << endm;
	

	StPicoDstMaker * picoDstMaker = (StPicoDstMaker*) GetMaker( "picoDst" );
	if ( nullptr == picoDstMaker ){
		LOG_INFO << "No StPicoDstMaker found, exit" << endm;
		return kStWarn;
	}

	this->_StPicoDst = picoDstMaker->picoDst();
	if ( nullptr == this->_StPicoDst ){
		LOG_INFO << "No StPicoDst found, exit" << endm;
		return kStWarn;
	}

	this->_StPicoEvent = this->_StPicoDst->event();
	if ( nullptr == this->_StPicoEvent ){
		LOG_INFO << "No StPicoEvent found, exit" << endm;
		return kStWarn;
	}
	

	
	// access primary vertex location
	// this->_StPicoEvent->primaryVertex().x()
	// this->_StPicoEvent->primaryVertex().y()
	// this->_StPicoEvent->primaryVertex().z()

	// common to cut on zvertex location
	// LOG_INFO << "vz = " << this->_StPicoEvent->primaryVertex().z() << endm;
	if ( fabs(this->_StPicoEvent->primaryVertex().z()) > 30.0 )
		return kStOK;

	float deltaVz = this->_StPicoEvent->vzVpd() - this->_StPicoEvent->primaryVertex().z();
	
	// cut o n diff between VPD and Primary vertex
	LOG_INFO << "dvz = " << deltaVz << endm;
	if ( fabs( deltaVz ) > 6.0  )
		return kStOK;

	// cut on ranking improves vertex quality
	LOG_INFO << "ranking = " << this->_StPicoEvent->ranking() << endm;
	if ( this->_StPicoEvent->ranking() < 0 )
		return kStOK;


	// loop on primary tracks
	for(unsigned int i=0;i<this->_StPicoDst->numberOfTracks();i++) {
		StPicoTrack *tr =  (StPicoTrack*) this->_StPicoDst->track(i);
		if(!tr) continue;

		analyzeTrack( tr );

	}

	return kStOK;

}



/* Analyze Track
 * See : http://www.star.bnl.gov/webdata/dox/html/classStPicoTrack.html
 * for info about what is inside an StPicoTrack
 */
void PicoDstSkimmer::analyzeTrack( StPicoTrack *picoTrack )
{
	if ( nullptr == picoTrack ){
		LOG_INFO << "WARN, null StPicoTrack" << endm;
		return;
	}

	StThreeVectorF primaryMomentum = picoTrack->pMom();
	if ( primaryMomentum.perp() < 0.001 ){
		//global tracks have zero primary momentum;
		return;
	}

	LOG_INFO << "(pT, eta, phi)=(" << primaryMomentum.perp() << ", " << primaryMomentum.pseudoRapidity() << ", " << primaryMomentum.phi() << ")" << endm;

	
}


