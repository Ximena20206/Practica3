% Datos simulados
tamanos = [1, 5, 10, 50, 1e3, 5e3, 10e3, 50e3, 1e6, 5e6, 1e7, 5e7, 1e9, 2e9, 5e9, 10e9, 50e9];
tiempos_codificacion = [
    0.0001, 0.0005, 0.0008, 0.0015, ...
    0.0123, 0.0234, 0.0345, 0.0789, ...
    0.2345, 0.6789, 1.1234, 5.6789, ...
    12.3456, 24.0000, 56.7890, 123.4567, 567.8901];
tiempos_decodificacion = [
    0.00012, 0.0006, 0.001, 0.0018, ...
    0.0154, 0.0276, 0.0387, 0.0901, ...
    0.2676, 0.7234, 1.2345, 6.2345, ...
    13.4567, 26.0000, 62.3456, 134.5678, 613.4567];

% Ajuste para compresión
log_tamanos = log(tamanos);
log_codificacion = log(tiempos_codificacion);
coef_codificacion = polyfit(log_tamanos, log_codificacion, 1);
% Función ajustada para compresión
ajuste_codificacion = exp(coef_codificacion(2)) * tamanos.^coef_codificacion(1);

% Gráfica para compresión
figure;
loglog(tamanos, tiempos_codificacion, 'bo-', 'LineWidth', 1.5);
hold on;
loglog(tamanos, ajuste_codificacion, 'b--', 'LineWidth', 1.5);
title('Aproximación de la función de complejidad para Compresión');
xlabel('Tamaño del archivo (Bytes)');
ylabel('Tiempo (segundos)');
legend('Datos simulados', sprintf('Ajuste: O(n^{%.2f})', coef_codificacion(1)));
grid on;

% Ajuste para descompresión
log_decodificacion = log(tiempos_decodificacion);
coef_decodificacion = polyfit(log_tamanos, log_decodificacion, 1);
% Función ajustada para descompresión
ajuste_decodificacion = exp(coef_decodificacion(2)) * tamanos.^coef_decodificacion(1);

% Gráfica para descompresión
figure;
loglog(tamanos, tiempos_decodificacion, 'ro-', 'LineWidth', 1.5);
hold on;
loglog(tamanos, ajuste_decodificacion, 'r--', 'LineWidth', 1.5);
title('Aproximación de la función de complejidad para Descompresión');
xlabel('Tamaño del archivo (Bytes)');
ylabel('Tiempo (segundos)');
legend('Datos simulados', sprintf('Ajuste: O(n^{%.2f})', coef_decodificacion(1)));
grid on;

% Mostrar ecuaciones ajustadas
disp('Ecuación para la compresión:');
fprintf('Tiempo = %.4e * Tamaño^{%.2f}\n', exp(coef_codificacion(2)), coef_codificacion(1));
disp('Ecuación para la descompresión:');
fprintf('Tiempo = %.4e * Tamaño^{%.2f}\n', exp(coef_decodificacion(2)), coef_decodificacion(1));
