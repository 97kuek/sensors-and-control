actual   = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100];
measured = [10, 19, 29, 37, 47, 56, 67, 77, 86,  96];

% 線形近似
p = polyfit(actual, measured, 1);
fit_x = linspace(0, 110, 100);
fit_y = polyval(p, fit_x);

% 決定係数 R^2
measured_fit = polyval(p, actual);
ss_res = sum((measured - measured_fit).^2);
ss_tot = sum((measured - mean(measured)).^2);
R2 = 1 - ss_res / ss_tot;

figure;
hold on;

plot(actual, measured, 'o', 'MarkerSize', 8, 'DisplayName', '測定値');
plot(fit_x, fit_y, '--', 'DisplayName', sprintf('近似直線: y = %.4fx + %.4f\nR^2 = %.4f', p(1), p(2), R2));

xlabel('実際の距離 [cm]');
ylabel('測定値 [cm]');
legend('Location', 'northwest');
grid on;
axis equal;
xlim([0 110]);
ylim([0 110]);

hold off;
