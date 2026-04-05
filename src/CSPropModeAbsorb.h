/*
*	Copyright (C) 2012-2025 Thorsten Liebig (Thorsten.Liebig@gmx.de)
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

#ifndef CSPROP_MODEABSORB_H
#define CSPROP_MODEABSORB_H

#include "CSProperties.h"
#include <string>

class CSXCAD_EXPORT CSPropModeAbsorb : public CSProperties
{
public:
	CSPropModeAbsorb(ParameterSet* paraSet) : CSProperties(paraSet) { Init(); }
	CSPropModeAbsorb(CSPropModeAbsorb* prop, bool copyPrim=false) : CSProperties(prop, copyPrim) { Init(); }
	CSPropModeAbsorb(unsigned int ID, ParameterSet* paraSet) : CSProperties(ID, paraSet) { Init(); }
	virtual ~CSPropModeAbsorb() {}

	virtual CSProperties* GetCopy(bool incl_prim=false) { return new CSPropModeAbsorb(this, incl_prim); }

	void SetNormalSignPositive(bool val) { m_NormalSignPositive = val; }
	bool GetNormalSignPositive() const { return m_NormalSignPositive; }

	void SetEModeFileName(const std::string& fileName) { m_EModeFileName = fileName; }
	std::string GetEModeFileName() const { return m_EModeFileName; }

	void SetHModeFileName(const std::string& fileName) { m_HModeFileName = fileName; }
	std::string GetHModeFileName() const { return m_HModeFileName; }

	void SetWaveImpedance(double val) { m_WaveImpedance = val; }
	double GetWaveImpedance() const { return m_WaveImpedance; }

	void SetUseModalFDTD(bool val) { m_UseModalFDTD = val; }
	bool GetUseModalFDTD() const { return m_UseModalFDTD; }

	void SetN1D(unsigned int val) { m_N1D = val; }
	unsigned int GetN1D() const { return m_N1D; }

	virtual const std::string GetTypeXMLString() const { return std::string("ModeAbsorb"); }

protected:
	void Init()
	{
		m_NormalSignPositive = true;
		m_EModeFileName = "";
		m_HModeFileName = "";
		m_WaveImpedance = 0;
		m_UseModalFDTD = false;
		m_N1D = 0;
	}

	bool m_NormalSignPositive;
	std::string m_EModeFileName;
	std::string m_HModeFileName;
	double m_WaveImpedance;
	bool m_UseModalFDTD;
	unsigned int m_N1D;
};

#endif // CSPROP_MODEABSORB_H
