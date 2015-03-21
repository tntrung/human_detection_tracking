function HMM=IntitHMM(transition,emission)
%struct('s',0.2,'a1',0.2,'a2',0.2,'a3',0.2,'a4',0.2);
%intit defaulse transition=[0.2 0.2 0.2 0.2 0.2], emission=[1 0 0 0 0]
HMM=struct('A',transition,'B',emission);