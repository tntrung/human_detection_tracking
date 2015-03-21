function HMM=UpdateHMM2(HMM,weight,index)
Parameter;
if index~=1
    HMM.A(1)=HMM.A(1)*weight*arrayHMM(1);    
    error=2;
    for i=1:3
        nearindex=index+i;
        if nearindex>9
            nearindex=nearindex-8;
        end
        HMM.A(nearindex)=HMM.A(nearindex)*weight*arrayHMM(error);    
        nearindex=index-i;
        if nearindex<2
           nearindex=nearindex+8; 
        end
        HMM.A(nearindex)=HMM.A(nearindex)*weight*arrayHMM(error);    
        error=error+1;
    end
    nearindex=index+4;
    if nearindex>9
        nearindex=nearindex-8; 
    end
    HMM.A(nearindex)=HMM.A(nearindex)*weight*arrayHMM(error);    
else
    for i=2:9
        HMM.A(i)=HMM.A(i)*weight;
    end    
end
%standard
HMM.A=HMM.A/sum(HMM.A);
%count
HMM.B(index)=HMM.B(index)+1;