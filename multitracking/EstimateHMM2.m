function value=EstimateHMM2(HMM)
value=zeros(1,9);
sumb = sum(HMM.B);
for i=1:9
    value(1,i)=HMM.A(i)*HMM.B(i)/sumb;    
end
%standard
value=value/sum(value);
%{
max=HMM.A(1)*HMM.B(1);
index=0;    %defaulse s
for i=2:5
    value=HMM.A(i)*HMM.B(i);
    if(max<value)
        max=value;
        index=i-1;
    end
end
%}