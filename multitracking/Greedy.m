function A=Greedy(S)
%input: S is score between tracker and detector by Score Function
%output: A(i,j)=1 : tracker i conect detect j else A(i,j)=0 : unconnect
Parameter;
sizes=size(S);
if sizes(1)==0 || sizes(2)==0
    A=0;
    return;
end
A=zeros(sizes(1),sizes(2));
mins=min(min(S))-1;
[gt,x,y]=GetIndexMax(S);     
min_d_tr=min(sizes(1),sizes(2));
numloop=0;
while gt>mins && numloop<min_d_tr    
    if gt>threshold_score        
        A(x,y)=1;
    end    
    S(x,:)=mins;
    S(:,y)=mins;    
    [gt,x,y]=GetIndexMax(S);     
    numloop=numloop+1;
end