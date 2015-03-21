function class=StandardMatrix_Row(class)
if ~isempty(class)    
    for i=1:length(class(:,1))
        row=class(i,:);
        maxrow=max(abs(row));
        if maxrow~=0
            row=row/maxrow;
        end
        class(i,:)=row;
    end
end