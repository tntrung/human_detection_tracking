function X = Bezier(point,NL)            
%{
a = 0:(1/(NL-1)):1;             %Bezier curve parameter
n = length(point(1,:));         %number of control points            
I = (0:(n-1))'*ones(1,NL);      %matrix of index values
P = ones(n,1)*a;                %matrix of probabilities
A = binopdf(I,n-1,P);           %weights for control points
X = point*A;                    %points on the Bezier curve
%}
X = point*(binopdf(((0:((length(point(1,:)))-1))'*ones(1,NL)),(length(point(1,:)))-1,(ones(length(point(1,:)),1)*(0:(1/(NL-1)):1))));