'''
python script for the projection of Nsigma distributions from THnSparses with PID var vs. ML output vs. pT
run: python CheckNsigmaDistrML.py cfgFileName.yml cutSetFileName.yml outFileName.root
'''

import sys
import argparse
import copy
import os
import numpy as np
import yaml
from PIL import Image
from ROOT import TFile, TCanvas, TDirectoryFile, TLegend  # pylint: disable=import-error,no-name-in-module
from ROOT import kBlue, kRed, kFullCircle, kOpenCircle  # pylint: disable=import-error,no-name-in-module
sys.path.append('../..')
from utils.TaskFileLoader import LoadPIDSparses  #pylint: disable=wrong-import-position,import-error
from utils.StyleFormatter import SetObjectStyle, SetGlobalStyle  #pylint: disable=wrong-import-position,import-error


# main function
SetGlobalStyle()
PARSER = argparse.ArgumentParser(description='Arguments to pass')
PARSER.add_argument('cfgFileName', metavar='text', default='cfgFileName.yml',
                    help='config file name with root input files')
PARSER.add_argument('cutSetFileName', metavar='text', default='cutSetFileName.yml',
                    help='input file with cut set')
PARSER.add_argument('outFileName', metavar='text', default='outFileName.root',
                    help='output root file name')
ARGS = PARSER.parse_args()
outFileNameWoExt = ARGS.outFileName.replace('.root', '')

with open(ARGS.cfgFileName, 'r') as ymlCfgFile:
    inputCfg = yaml.load(ymlCfgFile, yaml.FullLoader)

infilenames = inputCfg['filename']
if not isinstance(infilenames, list):
    infilenames = [infilenames]

for iFile, infilename in enumerate(infilenames):
    sPIDNsigmaTmp, sPIDNsigmaCombTmp, axes = LoadPIDSparses(infilename, inputCfg)
    if iFile == 0:
        sPIDNsigma = copy.deepcopy(sPIDNsigmaTmp)
        sPIDNsigmaComb = copy.deepcopy(sPIDNsigmaCombTmp)
    else:
        sPIDNsigma.Add(sPIDNsigmaTmp)
        sPIDNsigmaComb.Add(sPIDNsigmaCombTmp)

with open(ARGS.cutSetFileName, 'r') as ymlCutSetFile:
    cutSetCfg = yaml.load(ymlCutSetFile, yaml.FullLoader)
cutVars = cutSetCfg['cutvars']
MLmin, MLmax = [], []
for var in cutVars:
    if 'BDT' in var or 'ML' in var:
        MLmin = cutVars[var]['min']
        MLmax = cutVars[var]['max']
        break

if MLmin == [] or MLmax == []:
    print('ERROR: You did not set any ML selection! Exit')
    sys.exit()

hNsigma, hNsigmaSel = {}, {}
#1D distributions
for det in axes:
    hNsigma[det], hNsigmaSel[det] = {}, {}
    for spe in axes[det]:
        hNsigma[det][spe], hNsigmaSel[det][spe] = {}, {}
        for prong in axes[det][spe]:
            hNsigma[det][spe][prong], hNsigmaSel[det][spe][prong] = {}, {}
            for iPt, (ptmin, ptmax) in enumerate(zip(cutVars['Pt']['min'], cutVars['Pt']['max'])):

                ptbinmin = sPIDNsigma.GetAxis(0).FindBin(ptmin*1.0001)
                ptbinmax = sPIDNsigma.GetAxis(0).FindBin(ptmax*0.9999)

                MLbinmin = sPIDNsigma.GetAxis(1).FindBin(MLmin[iPt]*1.0001)
                MLbinmax = sPIDNsigma.GetAxis(1).FindBin(MLmax[iPt]*0.9999)


                if det != 'Comb':
                    sparse = sPIDNsigma
                else:
                    sparse = sPIDNsigmaComb

                sparse.GetAxis(0).SetRange(ptbinmin, ptbinmax)

                hNsigma[det][spe][prong][f'Pt{ptmin:.0f}_{ptmax:.0f}'] = \
                    sparse.Projection(axes[det][spe][prong])
                hNsigma[det][spe][prong][f'Pt{ptmin:.0f}_{ptmax:.0f}'].SetName(
                    f'hNsigma{det}_{spe}_Prong{prong}_Pt{ptmin:.0f}_{ptmax:.0f}')
                SetObjectStyle(hNsigma[det][spe][prong][f'Pt{ptmin:.0f}_{ptmax:.0f}'], color=kBlue,
                               linealpha=0.25, fillalpha=0.25, markeralpha=1, markerstyle=kFullCircle,
                               markersize=0.5, linewidth=1)

                sparse.GetAxis(1).SetRange(MLbinmin, MLbinmax)

                hNsigmaSel[det][spe][prong][f'Pt{ptmin:.0f}_{ptmax:.0f}'] = \
                    sparse.Projection(axes[det][spe][prong])
                hNsigmaSel[det][spe][prong][f'Pt{ptmin:.0f}_{ptmax:.0f}'].SetName(
                    f'hNsigmaSel{det}_{spe}_Prong{prong}_Pt{ptmin:.0f}_{ptmax:.0f}')
                SetObjectStyle(hNsigmaSel[det][spe][prong][f'Pt{ptmin:.0f}_{ptmax:.0f}'], color=kRed,
                               linealpha=0.25, fillalpha=0.25, markeralpha=1, markerstyle=kOpenCircle,
                               markersize=0.5, linewidth=1)

                sparse.GetAxis(0).SetRange(-1, -1)
                sparse.GetAxis(1).SetRange(-1, -1)

#2D distributions (TPC vs TOF)
hNsigma['TPCTOF'], hNsigmaSel['TPCTOF'] = {}, {}
for spe in axes['TPCTOF']:
    hNsigma['TPCTOF'][spe], hNsigmaSel['TPCTOF'][spe] = {}, {}
    for prong in axes['TPC'][spe]:
        hNsigma['TPCTOF'][spe][prong], hNsigmaSel['TPCTOF'][spe][prong] = {}, {}
        for iPt, (ptmin, ptmax) in enumerate(zip(cutVars['Pt']['min'], cutVars['Pt']['max'])):

            ptbinmin = sPIDNsigma.GetAxis(0).FindBin(ptmin*1.0001)
            ptbinmax = sPIDNsigma.GetAxis(0).FindBin(ptmax*0.9999)

            MLbinmin = sPIDNsigma.GetAxis(1).FindBin(MLmin[iPt]*1.0001)
            MLbinmax = sPIDNsigma.GetAxis(1).FindBin(MLmax[iPt]*0.9999)

            sPIDNsigma.GetAxis(0).SetRange(ptbinmin, ptbinmax)

            hNsigma['TPCTOF'][spe][prong][f'Pt{ptmin:.0f}_{ptmax:.0f}'] = \
                sPIDNsigma.Projection(axes['TPC'][spe][prong], axes['TOF'][spe][prong])
            hNsigma['TPCTOF'][spe][prong][f'Pt{ptmin:.0f}_{ptmax:.0f}'].SetName(\
                f'hNsigmaTPCTOF_{spe}_Prong{prong}_Pt{ptmin:.0f}_{ptmax:.0f}')
            SetObjectStyle(hNsigma['TPCTOF'][spe][prong][f'Pt{ptmin:.0f}_{ptmax:.0f}'], color=kBlue,
                           linealpha=0.25, fillalpha=0.25, markeralpha=1, markerstyle=kFullCircle,
                           markersize=0.3, linewidth=1)

            sPIDNsigma.GetAxis(1).SetRange(MLbinmin, MLbinmax)

            hNsigmaSel['TPCTOF'][spe][prong][f'Pt{ptmin:.0f}_{ptmax:.0f}'] = \
                sPIDNsigma.Projection(axes['TPC'][spe][prong], axes['TOF'][spe][prong])
            hNsigmaSel['TPCTOF'][spe][prong][f'Pt{ptmin:.0f}_{ptmax:.0f}'].SetName(\
                f'hNsigmaSelTPCTOF_{spe}_Prong{prong}_Pt{ptmin:.0f}_{ptmax:.0f}')
            SetObjectStyle(hNsigmaSel['TPCTOF'][spe][prong][f'Pt{ptmin:.0f}_{ptmax:.0f}'], color=kRed,
                           linealpha=0.25, fillalpha=0.25, markeralpha=1, markerstyle=kOpenCircle,
                           markersize=0.3, linewidth=1)

            sPIDNsigma.GetAxis(0).SetRange(-1, -1)
            sPIDNsigma.GetAxis(1).SetRange(-1, -1)

#2D distributions (prong0 vs prong2)
for det in axes:
    for spe in axes[det]:
        hNsigma[det][spe]['0-2'], hNsigmaSel[det][spe]['0-2'] = {}, {}
        for iPt, (ptmin, ptmax) in enumerate(zip(cutVars['Pt']['min'], cutVars['Pt']['max'])):

            ptbinmin = sPIDNsigma.GetAxis(0).FindBin(ptmin*1.0001)
            ptbinmax = sPIDNsigma.GetAxis(0).FindBin(ptmax*0.9999)

            MLbinmin = sPIDNsigma.GetAxis(1).FindBin(MLmin[iPt]*1.0001)
            MLbinmax = sPIDNsigma.GetAxis(1).FindBin(MLmax[iPt]*0.9999)


            if det != 'Comb':
                sparse = sPIDNsigma
            else:
                sparse = sPIDNsigmaComb

            sparse.GetAxis(0).SetRange(ptbinmin, ptbinmax)

            hNsigma[det][spe]['0-2'][f'Pt{ptmin:.0f}_{ptmax:.0f}'] = \
                sparse.Projection(axes[det][spe]['0'], axes[det][spe]['2'])
            hNsigma[det][spe]['0-2'][f'Pt{ptmin:.0f}_{ptmax:.0f}'].SetName(\
                f'hNsigma{det}_{spe}_Prong02_Pt{ptmin:.0f}_{ptmax:.0f}')
            SetObjectStyle(hNsigma[det][spe]['0-2'][f'Pt{ptmin:.0f}_{ptmax:.0f}'], color=kBlue,
                           linealpha=0.25, fillalpha=0.25, markeralpha=1, markerstyle=kFullCircle,
                           markersize=0.3, linewidth=1)

            sparse.GetAxis(1).SetRange(MLbinmin, MLbinmax)

            hNsigmaSel[det][spe]['0-2'][f'Pt{ptmin:.0f}_{ptmax:.0f}'] = \
                sparse.Projection(axes[det][spe]['0'], axes[det][spe]['2'])
            hNsigmaSel[det][spe]['0-2'][f'Pt{ptmin:.0f}_{ptmax:.0f}'].SetName(\
                f'hNsigmaSel{det}_{spe}_Prong02_Pt{ptmin:.0f}_{ptmax:.0f}')
            SetObjectStyle(hNsigmaSel[det][spe]['0-2'][f'Pt{ptmin:.0f}_{ptmax:.0f}'], color=kRed,
                           linealpha=0.25, fillalpha=0.25, markeralpha=1, markerstyle=kOpenCircle,
                           markersize=0.3, linewidth=1)

            sparse.GetAxis(0).SetRange(-1, -1)
            sparse.GetAxis(1).SetRange(-1, -1)


leg = TLegend(0.65, 0.76, 0.99, 0.89)
leg.SetTextSize(0.05)
leg.SetFillStyle(0)
leg.AddEntry(list(hNsigma['TPC']['Pi']['0'].values())[0], 'w/o ML sel', 'fp')
leg.AddEntry(list(hNsigmaSel['TPC']['Pi']['0'].values())[0], 'w/ ML sel', 'fp')

leg2D = TLegend(0.18, 0.76, 0.52, 0.89)
leg2D.SetTextSize(0.045)
leg2D.SetFillStyle(0)
leg2D.AddEntry(list(hNsigma['TPC']['Pi']['0'].values())[0], 'w/o ML sel', 'fp')
leg2D.AddEntry(list(hNsigmaSel['TPC']['Pi']['0'].values())[0], 'w/ ML sel', 'fp')

#plot on canvases and save on root file
listoffigures = []
outfile = TFile(ARGS.outFileName, 'recreate')
cNsigma, cNsigma02, cNsigmaNorm, dirNsigma = {}, {}, {}, {}
for iDet, det in enumerate(hNsigma):
    cNsigma[det] = {}
    cNsigmaNorm[det] = {}
    cNsigma02[det] = {}
    outfile.cd()
    dirNsigma[det] = TDirectoryFile(det, det)
    dirNsigma[det].Write()
    dirNsigma[det].cd()
    for iPt, (ptmin, ptmax) in enumerate(zip(cutVars['Pt']['min'], cutVars['Pt']['max'])):
        cNsigma[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'] = \
            TCanvas(f'cNsigma{det}_Pt{ptmin:.0f}_{ptmax:.0f}', '', 1920, 1080)
        cNsigma[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'].Divide(3, 2)
        cNsigmaNorm[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'] = \
            TCanvas(f'cNsigmaNorm{det}_Pt{ptmin:.0f}_{ptmax:.0f}', '', 1920, 1080)
        cNsigmaNorm[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'].Divide(3, 2)
        if det != 'TPCTOF':
            cNsigma02[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'] = \
                TCanvas(f'cNsigma02{det}_Pt{ptmin:.0f}_{ptmax:.0f}', '', 1920, 1080)
            cNsigma02[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'].Divide(2, 1)

        for iSpe, spe in enumerate(hNsigma[det]):
            for iProng, prong in enumerate(hNsigma[det][spe]):
                if prong != '0-2':
                    his = hNsigma[det][spe][prong][f'Pt{ptmin:.0f}_{ptmax:.0f}']
                    hissel = hNsigmaSel[det][spe][prong][f'Pt{ptmin:.0f}_{ptmax:.0f}']
                    his.GetXaxis().SetTitleSize(0.055)
                    hissel.GetXaxis().SetTitleSize(0.055)
                    his.GetXaxis().SetLabelSize(0.050)
                    hissel.GetXaxis().SetLabelSize(0.050)

                    if det != 'TPCTOF':
                        cNsigma[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'].cd(3*iSpe+iProng+1).SetLogy()
                        his.GetYaxis().SetRangeUser(0.8, his.GetMaximum()*10)
                        utilStr = (f'{ptmin} < #it{{p}}_{{T}} < {ptmax} GeV/#it{{c}};#it{{N}}_{{#sigma}}^{det} ({spe})'
                                   f' prong{iProng};Entries')
                        his.SetTitle(utilStr)
                        hissel.SetTitle(utilStr)
                        his.DrawCopy('hist')
                        his.DrawCopy('Esame')
                        hissel.DrawCopy('histsame')
                        hissel.DrawCopy('Esame')
                        leg.Draw('same')
                        his.Write()
                        hissel.Write()
                    else:
                        cNsigma[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'].cd(3*iSpe+iProng+1)
                        utilStr = (f'{ptmin} < #it{{p}}_{{T}} < {ptmax} GeV/#it{{c}};#it{{N}}_{{#sigma}}^{{TOF}} '
                                   f'({spe}) prong{iProng};#it{{N}}_{{#sigma}}^{{TPC}} ({spe}) prong{iProng}')
                        his.SetTitle(utilStr)
                        hissel.SetTitle(utilStr)
                        his.DrawCopy()
                        hissel.DrawCopy('same')
                        leg2D.Draw('same')
                        his.Write()
                        hissel.Write()
                        continue

                    hisnorm = his.Clone(his.GetName() + '_Norm')
                    hisselnorm = hissel.Clone(hissel.GetName() + '_Norm')

                    if hisnorm.Integral() > 0:
                        hisnorm.Scale(1./hisnorm.Integral())
                        hisnorm.GetYaxis().SetTitle('Normalised entries')
                        hisnorm.GetYaxis().SetRangeUser(1.e-5, 1.)

                    if hisselnorm.Integral() > 0:
                        hisselnorm.Scale(1./hisselnorm.Integral())
                        hisselnorm.GetYaxis().SetTitle('Normalised entries')
                        hisselnorm.GetYaxis().SetRangeUser(1.e-5, 1.)

                    cNsigmaNorm[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'].cd(3*iSpe+iProng+1).SetLogy()
                    hisnorm.DrawCopy('hist')
                    hisnorm.DrawCopy('Esame')
                    hisselnorm.DrawCopy('histsame')
                    hisselnorm.DrawCopy('Esame')
                    leg.Draw('same')

                    cNsigmaNorm[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'].cd(3*iSpe+iProng+1).Modified()
                    cNsigmaNorm[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'].cd(3*iSpe+iProng+1).Update()
                    hisnorm.Write()
                    hisselnorm.Write()

                cNsigma[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'].cd(3*iSpe+iProng+1).Modified()
                cNsigma[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'].cd(3*iSpe+iProng+1).Update()

            if det != 'TPCTOF':
                his = hNsigma[det][spe][prong][f'Pt{ptmin:.0f}_{ptmax:.0f}']
                hissel = hNsigmaSel[det][spe][prong][f'Pt{ptmin:.0f}_{ptmax:.0f}']
                his.GetXaxis().SetTitleSize(0.055)
                hissel.GetXaxis().SetTitleSize(0.055)
                his.GetXaxis().SetLabelSize(0.050)
                hissel.GetXaxis().SetLabelSize(0.050)

                cNsigma02[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'].cd(iSpe+1)
                utilStr = (f'{ptmin} < #it{{p}}_{{T}} < {ptmax} GeV/#it{{c}};#it{{N}}_{{#sigma}}^{det} ({spe}) '
                           f'prong0;#it{{N}}_{{#sigma}}^{det} ({spe}) prong2')
                his.SetTitle(utilStr)
                hissel.SetTitle(utilStr)
                his.DrawCopy()
                hissel.DrawCopy('same')
                leg2D.Draw('same')

        cNsigma[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'].Write()
        if det != 'TPCTOF':
            cNsigmaNorm[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'].Write()
            cNsigma02[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'].Write()

        #save also pdf
        cNsigma[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'].SaveAs(f'{outFileNameWoExt}{det}{iPt}.png')
        listoffigures.append(f'{outFileNameWoExt}{det}{iPt}.png')

        if det != 'TPCTOF':
            cNsigmaNorm[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'].SaveAs(f'{outFileNameWoExt}{det}{iPt}Norm.png')
            cNsigma02[det][f'Pt{ptmin:.0f}_{ptmax:.0f}'].SaveAs(f'{outFileNameWoExt}{det}{iPt}02.png')
            listoffigures.append(f'{outFileNameWoExt}{det}{iPt}Norm.png')
            listoffigures.append(f'{outFileNameWoExt}{det}{iPt}02.png')

outfile.Close()

# combine png files
figures = [Image.open(figures) for figures in listoffigures]
min_shape = sorted([(np.sum(fig.size), fig.size) for fig in figures])[0][1]
imgs_comb = np.vstack(figures)
imgs_comb = Image.fromarray(imgs_comb)
imgs_comb.save(f'{outFileNameWoExt}.png')

for fig in listoffigures:
    os.remove(fig)

input('Press enter to exit')
