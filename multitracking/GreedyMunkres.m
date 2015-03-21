function A=GreedyMunkres(S)
Parameter;
sizes=size(S);
if sizes(1)==0 || sizes(2)==0
    A=0;
    return;
end
A=zeros(sizes(1),sizes(2));
value=Munkres(max(max(S))-S);
for i=1:sizes(1)    
    if value(i)~=0 && S(i,value(i))>threshold_score
        A(i,value(i))=1;
    end
end