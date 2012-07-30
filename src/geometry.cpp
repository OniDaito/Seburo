/**
* @brief Geometry conversion and misc functions
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 30/07/2012
*
*/

#include "geometry.hpp"


using namespace std;
using namespace boost;
using namespace boost::assign;

/*
 * Convert Geometry from basic to Leeds 8 texture version
 */ 
namespace s9 {

void convertGeometry( Geometry<VertPNF> a, Geometry<VertPNT8F> b) {

	vector<VertPNT8F> vtemp;
	vector<VertPNF> tv = a.getBuffer();
	BOOST_FOREACH(VertPNF p, tv) {

		VertPNT8F tp;
		tp.mP = p.mP;
		tp.mN = p.mN;

		for (int i = 0; i < 8; i ++){
			Float2 tt; tt.x = 0.0f; tt.y = 0.0f;
			tp.mT[i] = tt;
		}
		vtemp += tp;
	}

	b = Geometry<VertPNT8F> (vtemp);
	vector<uint32_t> ti = a.getIndices();
	b.addIndices(ti);
}

}
