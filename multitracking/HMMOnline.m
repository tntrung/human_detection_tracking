%demo HMM online
%transition defaule and emission
HMM=IntitHMM([0.2 0.2 0.2 0.2 0.2],[0 0 0 0 0]);
%sequence stage: 
%s -1-> s -0.5-> 1 -0.5-> 1 -0.2-> 2 -0.2-> 1
HMM=UpdateHMM(HMM,1,0);
HMM=UpdateHMM(HMM,0.5,1);
HMM=UpdateHMM(HMM,0.5,1);
HMM=UpdateHMM(HMM,0.2,2);
HMM=UpdateHMM(HMM,0.2,1);
%estimate
index=EstimateHMM(HMM);
%update output
HMM=UpdateHMM(HMM,0.5,index);