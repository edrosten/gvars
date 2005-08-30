#include "gvars3/serialize.h"
#include <vector>

using namespace std;
using namespace TooN;


namespace GVars3
{
namespace serialize
{
	string to_string(const Matrix<>& m)
	{
		ostringstream o;

		o << "[ ";

		for(int i=0; i < m.num_rows(); i++)
		{
			o << m[i];
			if(i < m.num_rows() - 1)
				o << ";";
		}
		o << "]";
		return o.str();
	}



	bool from_string(string s, Matrix<>& mat)
	{
		 // Format expected is: [ num .. num; ... ;  num .. num ]
		int n;

		//First, find opening angle bracket.
		//Trim up to and including this. Abort if it can't be found.
		n=s.find("[");
		if(n==s.npos) 
			return false;
		s.erase(0, n+1 );

		// Trim the closing brace and rest of line. Again, abort if can't be found.
		n=s.find("]");
		if(n==s.npos) 
			return false;
		s.erase(n, s.npos-n );

		vector<double> vd;  // Store all the numbers in this here.

		int nRows=1;
		int nCols;
		string sub;
		double d;

		n=s.find(";");   // Read out the first line. Might be the last.
		{
			sub=s.substr(0,n);
			istringstream ist(sub);
			while(ist>>d)
				vd.push_back(d);
		};

		nCols=vd.size();
		if(nCols<1) 
			return false;

		while(n!=s.npos)  // This means that n pointed to a semicolon, not the end of the string
		{               // Ergo: at least another line to go.
			nRows++;
			s.erase(0,n+1);
			n=s.find(";");   
			sub=s.substr(0,n);
			istringstream ist(sub);
			while(ist>>d)
				vd.push_back(d);

			if(vd.size()!=nRows*nCols) // Were there the right number of elements?
				return false;
		};

		// Now construct and fill the matrix.
		Matrix<> m(nRows, nCols);

		for(int i=0;i<nRows;i++)
			for(int j=0;j<nCols;j++)
				m(i,j)=vd[i*nCols+j];

		mat = m;

		return true;
	}


	string to_string(const Vector<>& m)
	{
		ostringstream o;

		o << "[ ";
		o << m;
		o << "]";
		return o.str();
	}


	bool from_string(string s, Vector<>& vec)
	{
		// Format expected is: [ num num num .. num ]
		int n;

		//First, find opening angle bracket.
		//Trim up to and including this. Abort if it can't be found.
		n=s.find("[");
		if(n==s.npos)
			return false;

		s.erase(0, n+1 );
		// Trim the closing brace and rest of line. Again, abort if can't be found.
		n=s.find("]");
		if(n==s.npos)
			return false;
		s.erase(n, s.npos-n );

		//Now, we can read some numbers! Turn into a stream, and build an STL-vector of doubles.
		istringstream ist(s);
		double d;
		vector<double> vd;

		while(ist>>d)
			vd.push_back(d);

		if(vd.size()<1)// Abort if vector was empty.
			return false;

		// Now transfer the STL-vector over to a TooN - Vector.
		Vector<> v(vd.size());

		for(int i=0;i<vd.size();i++)
			v[i]=vd[i];

		return true;
	}


}


}
