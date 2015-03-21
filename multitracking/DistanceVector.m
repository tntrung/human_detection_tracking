function disttrd=DistanceVector(t,d)
%input: rect[point(track),vector] (x,y,u,v), point (deletec) (x,y)
%output: distance track and detector by vector track
Parameter;
if DistantPoint([t(1) t(2)],[t(1)+t(3) t(2)+t(4)])<threshold_vector_gate || (d(1)-t(1))*t(3)<0 || (d(2)-t(2))*t(4)<0
    %if |vector| < threshold_vector_gate: use distance point to point
    %and if vector t nam cung phia voi detector vi tri so sanh la tracker
    disttrd=DistantPoint([t(1) t(2)],[d(1) d(2)]);    
else
    %if |vector| >= threshold_vector_gate: use distance point to vector
    disttrd = DistandPointToLine([d(1) d(2) 0],[t(1) t(2) 0],[t(1)+t(3) t(2)+t(4) 0]);     
end
%{
Parameter;
if (t.u * t.u + t.v * t.v)<(threshold_vector_gate*threshold_vector_gate)    
    %if under threshold vector: compute distance point
    disttrd=DistantPoint([t.cx t.cy],[d(1)+d(3)/2 d(2)+d(4)/2]);    
else
    %compute distance point to line
    disttrd = DistandPointToLine([d(1)+d(3)/2 d(2) + d(4)/2 0],[t.cx , t.cy 0],[t.u + t.cx , t.v + t.cy 0]);     
end
%}