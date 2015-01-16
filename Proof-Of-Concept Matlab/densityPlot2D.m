function densityPlot2D(xx,yy,gridSize)

% densityPlot2D(xx,yy,gridSize) takes the x and y data vectors and a grid size on 
% which to compute the density plot as inputs. It computes the 2D histogram
%, which is the number of points in each grid. TRhen it smoothes this 2D
% histogram by fitting with a durface presents the surface in a plot.

% choose a colormap
colormap(cool);

% declare the grid
x=linspace(min(xx),max(xx),gridSize);
y=linspace(min(yy),max(yy),gridSize);
gridEval = zeros(length(x)-1,length(y)-1);

% compute frequency of pooints in each grid 
for cnt_x=1:length(x)-1
    for cnt_y=1:length(y)-1
        x_ind=intersect(find(xx>x(cnt_x)),find(xx<=x(cnt_x+1)));                                                    
        xy_ind=intersect(find(yy(x_ind)>y(cnt_y)), find(yy(x_ind)<=y(cnt_y+1)));     
        gridEval(cnt_y, cnt_x)=length(xy_ind);
    end
end

% fit surface to the 2D histogram
surf((x(1:end-1)+ x(2:end))/2,(y(1:end-1)+y(2:end))/2,gridEval); view(2); 
shading interp;  hold on;
axis([min(xx),max(xx) min(yy),max(yy)]);

% add annotations
title(['Density Plot, Grid size: ' num2str(gridSize) ' x ' num2str(gridSize) ' grid'],'Fontsize',14);
xlabel('x','Fontsize',14);
ylabel('y','Fontsize',14);
axis tight;
h1 = gca; % save the handle to current axis so you can draw a box around it later

% add a colorbar
h=colorbar;
axes(h);ylabel('Density, #points per unit grid','Fontsize',14);
set(gcf,'color',[1 1 1],'paperpositionmode','auto');

% put a border around the graph with black lines
axes(h1);
line(get(gca,'xlim'),repmat(min(get(gca,'ylim')),1,2),'color',[0 0 0],'linewidth',1);
line(get(gca,'xlim'),repmat(max(get(gca,'ylim')),1,2),'color',[0 0 0],'linewidth',2);
line(repmat(min(get(gca,'xlim')),1,2),get(gca,'ylim'),'color',[0 0 0],'linewidth',2);
line(repmat(max(get(gca,'xlim')),1,2),get(gca,'ylim'),'color',[0 0 0],'linewidth',1);
