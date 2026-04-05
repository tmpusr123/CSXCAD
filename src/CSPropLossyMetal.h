/*
*	Copyright (C) 2008-2025 Thorsten Liebig (Thorsten.Liebig@gmx.de)
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU Lesser General Public License as published
*	by the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU Lesser General Public License for more details.
*
*	You should have received a copy of the GNU Lesser General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "CSPropMetal.h"

//! Continuous Structure Lossy Metal Property
/*!
  A lossy metal model for 3D conductor volumes using Surface Impedance
  Boundary Condition (SIBC). The interior is PEC (from METAL flag),
  and the surface gets frequency-dependent conductor loss via SIBC.
  This models semi-infinite (bulk) conductors, unlike ConductingSheet
  which models thin 2D sheets.
  */
class CSXCAD_EXPORT CSPropLossyMetal : public CSPropMetal
{
public:
	CSPropLossyMetal(ParameterSet* paraSet);
	CSPropLossyMetal(CSPropLossyMetal* prop, bool copyPrim=false);
	CSPropLossyMetal(unsigned int ID, ParameterSet* paraSet);
	virtual ~CSPropLossyMetal();

	//! Create a copy of this property. Optional: Copy all primitives assigned to this property too.
	virtual CSProperties* GetCopy(bool incl_prim=false) {return new CSPropLossyMetal(this, incl_prim);}

	virtual void Init();

	//! Get PropertyType as a xml element name \sa PropertyType and GetType
	virtual const std::string GetTypeXMLString() const {return std::string("LossyMetal");}

	//! Set the Conductivity
	void SetConductivity(double val) {Conductivity.SetValue(val);}
	//! Set the Conductivity
	int  SetConductivity(const std::string val)  {return Conductivity.SetValue(val);}
	//! Get the Conductivity
	double GetConductivity() {return Conductivity.GetValue();}
	//! Get the Conductivity as a string
	const std::string GetConductivityTerm() {return Conductivity.GetString();}

	virtual bool Update(std::string *ErrStr=NULL);

	virtual bool Write2XML(TiXmlNode& root, bool parameterised=true, bool sparse=false);
	virtual bool ReadFromXML(TiXmlNode &root);

	virtual void ShowPropertyStatus(std::ostream& stream);

protected:
	ParameterScalar Conductivity;
};
