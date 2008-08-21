/*                       
	This file is part of the GVars3 Library.

	Copyright (C) 2005 The Authors

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 
    51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

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

		o << "[";

		for(int i=0; i < m.num_rows(); i++)
		  {
		    for(int j=0; j < m.num_cols(); j++)
		      o << " " << m[i][j];
		    if(i < m.num_rows() - 1)
		      o << " ;";
		  }
		o << " ]";
		return o.str();
	}



	int from_string(string s, Matrix<>& mat)
	{
		 // Format expected is: [ num .. num; ... ;  num .. num ]
	        string::size_type n;

		//First, find opening angle bracket.
		//Trim up to and including this. Abort if it can't be found.
		n=s.find("[");
		if(n==s.npos) 
			return 1;
		s.erase(0, n+1 );

		// Trim the closing brace and rest of line. Again, abort if can't be found.
		n=s.find("]");
		if(n==s.npos) 
			return 1;
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
			return 1;

		while(n!=s.npos)  // This means that n pointed to a semicolon, not the end of the string
		{               // Ergo: at least another line to go.
			nRows++;
			s.erase(0,n+1);
			n=s.find(";");   
			sub=s.substr(0,n);
			istringstream ist(sub);
			while(ist>>d)
				vd.push_back(d);

			if(static_cast<int>(vd.size())!=nRows*nCols) // Were there the right number of elements?
				return 1;
		};

		// Now construct and fill the matrix.
		Matrix<> m(nRows, nCols);

		for(int i=0;i<nRows;i++)
			for(int j=0;j<nCols;j++)
				m(i,j)=vd[i*nCols+j];
		
		mat.resize(m.num_rows(), m.num_cols());
		mat = m;

		return 0;
	}


	string to_string(const Vector<>& m)
	{
		ostringstream o;

		o << "[ ";
		for(int i=0; i<m.size(); i++)
		  o << m[i] << " ";
		o << "]";
		return o.str();
	}


	int from_string(string s, Vector<>& vec)
	{
		// Format expected is: [ num num num .. num ]
	        string::size_type n;

		//First, find opening angle bracket.
		//Trim up to and including this. Abort if it can't be found.
		n=s.find("[");
		if(n==s.npos)
			return 1;

		s.erase(0, n+1 );
		// Trim the closing brace and rest of line. Again, abort if can't be found.
		n=s.find("]");
		if(n==s.npos)
			return 1;

		s.erase(n, s.npos-n );

		//Now, we can read some numbers! Turn into a stream, and build an STL-vector of doubles.
		istringstream ist(s);
		double d;
		vector<double> vd;

		while(ist>>d)
			vd.push_back(d);

		if(vd.size()<1)// Abort if vector was empty.
			return 1;

		// Now transfer the STL-vector over to a TooN - Vector.
		Vector<> v(vd.size());

		for(size_t i=0;i<vd.size();i++)
			v[i]=vd[i];
		
		vec.resize(v.size());
		vec = v;

		return 0;
	}

	std::string to_string(const std::string& s)
	{
		return s;
	}

        // For reading strings, if there's double quotes, lift the part inside the double quotes.
        int from_string(std::string s, std::string& so) //Pretty ugly code lifted from GVars2, but works.
	{
	  unsigned char c;
	  int nPos=0;
	  int nLength = s.length();
	  
	  // Find Start....
	  bool bFoundOpenQuote = false;
	  while((nPos < nLength) && !bFoundOpenQuote)
	    {
	      c = s[nPos];
	      if(c=='"') bFoundOpenQuote =true;
	      nPos++;
	    }
	  if(!bFoundOpenQuote) // No quotes found - assume that the entire thing is good to go...
	    {
	      so = s;
	      return 0;
	    }
	  // Found opening ". Copy until end, or closing "
	  
	  std::string sNew("");
	  for (; nPos < nLength; ++nPos) {
	      char c = s[nPos];
	      if (c == '"') {
		  ++nPos;
		  break;
	      }
	      if (nPos+1<nLength && c == '\\') {
		  char escaped = s[++nPos];
		  switch (escaped) {
		  case 'n': c = '\n'; break;
		  case 'r': c = '\r'; break;
		  case 't': c = '\t'; break;
		  default: c = escaped; break;
		  }
	      }
	      sNew.push_back(c);
	  }
	  if (nPos < nLength) {
	      string rest;
	      from_string(s.substr(nPos), rest);
	      sNew += rest;
	  }

	  so = sNew;
	  return 0;
	}
  

	int check_stream(std::istream& i)
	{
		if(i.bad())
			return 1;

		if(i.good())
		{
			i >> ws;
			char c = i.get();

			if(!i.eof())
			{	
				i.putback(c);
				return -i.tellg();
			}
		}

		return 0;
	}

}


}
