function d=DistandPointToLine(pt,v1,v2)
%input: point[px py], line[point(lx,ly),point(lu,lv)]
a = v1 - v2;
b = pt - v2;
d = norm(cross(a,b)) / norm(a);