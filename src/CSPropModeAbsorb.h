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
#include <vector>

class CSXCAD_EXPORT CSPropModeAbsorb : public CSProperties
{
public:
	// IMPORTANT: must set Type = MODE_ABSORB so this property registers
	// under that bitmask. Without it, openEMS::SetupModeAbsorbers() never
	// runs (it's gated on `GetQtyPropertyType(MODE_ABSORB) > 0`), and the
	// 1D modal-FDTD / matched-modal absorber never builds — silently.
	CSPropModeAbsorb(ParameterSet* paraSet) : CSProperties(paraSet) { Type = MODE_ABSORB; Init(); }
	CSPropModeAbsorb(CSPropModeAbsorb* prop, bool copyPrim=false) : CSProperties(prop, copyPrim) { Type = MODE_ABSORB; Init(); }
	CSPropModeAbsorb(unsigned int ID, ParameterSet* paraSet) : CSProperties(ID, paraSet) { Type = MODE_ABSORB; Init(); }
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

	// Modal cutoff wavenumber kc [rad/m] = 2*pi*f_cutoff / c. Required by
	// the per-mode 1-D Yee absorber (Operator_Ext_ModalFDTD) for the
	// Klein-Gordon mass term. The matched-modal-source absorber
	// (Operator_Ext_ModeAbsorb, the older path) does not use this and
	// can leave it 0.
	void SetKc(double val) { m_Kc = val; }
	double GetKc() const { return m_Kc; }

	// Port amplitude (excitation strength). Read by Engine_Ext_ModeAbsorb
	// to compute the known incident modal amplitude from the openEMS
	// Excitation signal:  a_inc(t) = 0.5 * Signal[n] * K_eff * amplitude.
	// For passive ports (amplitude=0) the subtract-incident logic is a
	// no-op and the absorber falls back to pure matched-modal-source.
	void SetPortAmplitude(double val) { m_PortAmplitude = val; }
	double GetPortAmplitude() const { return m_PortAmplitude; }

	// State-space port operator (CST CBBPortOperatorStateSpace equivalent,
	// 2026-05-07).  When UseStateSpace=true, the absorber replaces the
	// V/I matched-modal computation a_bwd = 0.5(Emm − dir·Zw·Hmm) with
	// a vector-fitted state-space recursion that correctly handles the
	// modal frequency dependence (propagating + evanescent regimes).
	// At each timestep with input u_n = Emm:
	//     ψ_new = SS_P · ψ + SS_Q · u_n            (Crank-Nicolson advance,
	//                                               2·u_n approx for trapezoidal source)
	//     y_n   = SS_C · ψ_new + SS_D · u_n         (matched-termination amplitude)
	//     correction = (u_n − y_n) · e_mode         (subtracted from E at Π)
	//
	//   SS_P = (I − 0.5·dt·A)⁻¹·(I + 0.5·dt·A)   shape n×n (row-major)
	//   SS_Q = (I − 0.5·dt·A)⁻¹·dt·B             shape n×1
	//   SS_C =                                    shape 1×n
	//   SS_D =                                    scalar
	// (A,B,C,D) come from Gustavsen vector fit of tanh(γ_m(ω)·Δz) target.
	// SS_n = state vector size.  Matrices stored row-major as flat
	// std::vector<double> for transparent Cython binding.
	void SetUseStateSpace(bool val) { m_UseStateSpace = val; }
	bool GetUseStateSpace() const { return m_UseStateSpace; }

	void SetSSOrder(unsigned int n) { m_SS_n = n; }
	unsigned int GetSSOrder() const { return m_SS_n; }

	void SetSSP(const std::vector<double>& v) { m_SS_P = v; }
	void SetSSQ(const std::vector<double>& v) { m_SS_Q = v; }
	void SetSSC(const std::vector<double>& v) { m_SS_C = v; }
	void SetSSD(double val) { m_SS_D = val; }
	const std::vector<double>& GetSSP() const { return m_SS_P; }
	const std::vector<double>& GetSSQ() const { return m_SS_Q; }
	const std::vector<double>& GetSSC() const { return m_SS_C; }
	double GetSSD() const { return m_SS_D; }

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
		m_Kc = 0;
		m_PortAmplitude = 0;
		m_UseStateSpace = false;
		m_SS_n = 0;
		m_SS_D = 0;
		m_SS_P.clear();
		m_SS_Q.clear();
		m_SS_C.clear();
	}

	bool m_NormalSignPositive;
	std::string m_EModeFileName;
	std::string m_HModeFileName;
	double m_WaveImpedance;
	bool m_UseModalFDTD;
	unsigned int m_N1D;
	double m_Kc;
	double m_PortAmplitude;

	bool m_UseStateSpace;
	unsigned int m_SS_n;
	std::vector<double> m_SS_P;     // n×n row-major
	std::vector<double> m_SS_Q;     // n×1
	std::vector<double> m_SS_C;     // 1×n
	double m_SS_D;
};

#endif // CSPROP_MODEABSORB_H
