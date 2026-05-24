#ifndef ANALYZE_HH
#define ANALZE_HH

class Analyze{

	public:
		void LoadPlastic(); //Forming Plastic Hits
		
		void LoadDC();

		void MakeDCHits();  // Group Wires, Create STC and Make Position
		void MakeGroups(TClonesArray* dcstrips, TClonesArray* dcstriproups); //Group the Cathode in MakeDCHits 
		void MakePositionCathode(TClonesArray* dcstripgroup,TClonesArray* cathodeposition); //Input Should be Group
		void MakeDCPosition(TClonesArray *KUgroups,TClonesArray *KVgrousp,  TVector3 *dcpos); // 
		void MakeTracks(); //Make Tracks and plot 
		void ReconstructSTC();
		void ReconstructTracks();

		void Clear();


	private:

		TClonesArray *tracks;
		TClonesArray *DCHits;
		TClonesArray *GroupHits;
		TClonesArray *DCWireHits;


};
