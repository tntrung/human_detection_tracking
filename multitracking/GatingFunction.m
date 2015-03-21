function outfloat=GatingFunction(tr,d)
Parameter;
sizetr=tr.width*tr.height;
sized=d(3)*d(4);
outfloat=gaussmf((sizetr-sized)/sizetr,[sigma_gate mean_gate]);
disttrd=DistanceVector([tr.cx tr.cy tr.u tr.v],[d(1)+d(3)/2 d(2)+d(4)/2]);
outfloat = outfloat * gaussmf( disttrd , [sigma_gate mean_gate] );