function HMM=UpdateHMM(HMM,weight,index)
if index==0     %s
    HMM.A(2)=HMM.A(2)*weight;
    HMM.A(3)=HMM.A(3)*weight;
    HMM.A(4)=HMM.A(4)*weight;
    HMM.A(5)=HMM.A(5)*weight;    
    %standard
    HMM.A=HMM.A/sum(HMM.A);
    %count
    HMM.B(1)=HMM.B(1)+1;
elseif index==1 %a1
    HMM.A(1)=HMM.A(1)*weight;
    %HMM.A(2)=HMM.A(2)*weight;
    HMM.A(3)=HMM.A(3)*weight*0.9;
    HMM.A(4)=HMM.A(4)*weight*0.7;
    HMM.A(5)=HMM.A(5)*weight*0.9;    
    %standard
    HMM.A=HMM.A/sum(HMM.A);
    %count
    HMM.B(2)=HMM.B(2)+1;
elseif index==2 %a2
    HMM.A(1)=HMM.A(1)*weight;
    HMM.A(2)=HMM.A(2)*weight*0.9;
    %HMM.A(3)=HMM.A(3)*weight;
    HMM.A(4)=HMM.A(4)*weight*0.9;
    HMM.A(5)=HMM.A(5)*weight*0.7;    
    %standard
    HMM.A=HMM.A/sum(HMM.A);
    %count
    HMM.B(3)=HMM.B(3)+1;
elseif index==3 %a3
    HMM.A(1)=HMM.A(1)*weight;
    HMM.A(2)=HMM.A(2)*weight*0.7;
    HMM.A(3)=HMM.A(3)*weight*0.9;
    %HMM.A(4)=HMM.A(4)*weight;
    HMM.A(5)=HMM.A(5)*weight*0.9;    
    %standard
    HMM.A=HMM.A/sum(HMM.A);
    %count
    HMM.B(4)=HMM.B(4)+1;
else            %a4    
    HMM.A(1)=HMM.A(1)*weight;
    HMM.A(2)=HMM.A(2)*weight*0.9;
    HMM.A(3)=HMM.A(3)*weight*0.7;
    HMM.A(4)=HMM.A(4)*weight*0.9;
    %HMM.A(5)=HMM.A(5)*weight;    
    %standard
    HMM.A=HMM.A/sum(HMM.A);
    %count
    HMM.B(5)=HMM.B(5)+1;
end