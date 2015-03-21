function [a,maxa]=StandardMatrix(a,maxa)
%input: matrix a, value max
%output: matrix standard
maxm=max(max(a));   %get max in matrix a
if maxm>maxa        %if max is not larger
    maxa=maxm;      %update max
end
if maxa~=0
   a=a/maxa;        %standard
end