# DiMuonKF_ISR_Fit
Fit of the DiMuon sqrt(s) tail (lower energy tail) for the ILC 250 GeV e+e- beam. Uses Kuraev-Fadin's analytical ISR distribution to fit. Includes a 1 parameter fit, which only fits for mean beam energy sqrt(s). There is also a 3 parameter fit which changes fine structure constant, lepton mass and mean beam energy.

Preamble:

The data fit for was generated using ILCSOFT, which is simulation software for ILC.

This data includes effects from beamstrahlung as well as beam energy spread and detector resolution. Still, this fit was done to see how well the Kuraev-Fadin Initial State Radiation (ISR) distribution fit and how much the parameters would change for future analysis purposed. See https://www.semanticscholar.org/paper/On-Radiative-Corrections-to-e%2B-e-Single-Photon-at-Kuraev-Fadin/908259979e8e844f5e8a7f44c3e2a10f40645751 for original citation of Kuraeve-Fadin (KF) as well as this paper (https://www.sciencedirect.com/science/article/pii/S0370269316306566?via%3Dihub) for the written form of the fit and the notation.

Fitting was done using ROOT v6.21 and RooFit.

During the fit process the range of the fitted data must not exceed or be equal to 250 GeV. Fitting for the KF distribution at 250 GeV leads to a discontinuity at 250 GeV and the fit minimizer is unable to converge to a valid fit. Thus a value of max value of 249.9 GeV was used.

Results: 

One remark -- The KF fit for 1 and 3 parameter resulted in keeping the fine structure constant as just that, a constant. Despite not requiring this RooFit chose to make it a constant. Which I've actually never seen before so its interesting that it can automatically do that! It kept it a constant at 1/137 rounded to 5 decimals.

Similarly the lepton mass, when it was let to be free in the 3 parameter fit, stayed remarkably close to the muon mass. Which are the leptons in question here. For reference the Muon mass is about 105.7 MeV.

Table 1:

| #Params in KF | ChiSq Stat    | Deg. of Free. | Fine Struct.  | Error   | Lepton Mass (MeV)   | Error   | Mean Beam Energy (GeV) | Error   |
| ------------- | ------------- | ------------- | ------------- | ------- | ------------------- | ------- | ---------------------- | ------- |
| 1 Parameter   | 161238        | 199           | 0.00073       |  Fixed  | 105.7               |  Fixed  | 250.12                 | 0.15    |
| 3 Parameter   | 17001         | 197           | 0.00073       |  Fixed  | 198.6               |  3.7    | 250.18                 | 0.21    |

Remarks:

Considering that the data contains more than ISR, like beamstrahlung, it is interesting that the fit doesn't do too terribly in the 3 parameter. The reduced ChiSq is ~86.

Perhaps the lepton mass is meant to be 2 muon rest mass and not 1? Need to fit only ISR data or a combination of an ISR and a beamstrahlung distribution (like CIRCE) and see what happens...
