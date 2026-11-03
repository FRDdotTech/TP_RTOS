%% Identification fonction de transfert - boîtier papillon
% Fichiers : step_0_10.csv, step_0_20.csv, ..., step_0_70.csv
% Format :
%   temps_ms, raw response, valeur_ADC
% Exemple :
%   000000, raw response, 02304
%
% Hypothèses :
% - 500 échantillons = 500 ms
% - Fréquence d'échantillonnage = 1 kHz
% - L'échelon de duty cycle est appliqué à t = 0 ms
% - Le duty initial est 0 %
% - Le duty final est donné par le nom du fichier

clear; clc; close all;

%% Calibration ADC
ADC_0   = 1955;   % ADC à 0 % de duty / papillon fermé
ADC_100 = 2850;   % ADC à 100 % de duty / papillon ouvert

%% Liste des essais
% Fichier             duty initial [%]   duty final [%]
files = {
    'step_0_10.csv',  0, 10;
    'step_0_20.csv',  0, 20;
    'step_0_30.csv',  0, 30;
    'step_0_40.csv',  0, 40;
    'step_0_50.csv',  0, 50;
    'step_0_60.csv',  0, 60;
    'step_0_70.csv',  0, 70
};

nTests = size(files, 1);

%% Paramètres d'acquisition
Fs = 1000;          % Hz
Ts = 1 / Fs;        % s, soit 1 ms
N_expected = 500;   % nombre de points attendus par fichier
T_record = N_expected * Ts;   % 0.5 s

%% Figures et tableaux de résultats
figure('Name', 'Réponses indicielles mesurées');
hold on;

results = table(...
    strings(nTests,1), ...
    zeros(nTests,1), ...
    zeros(nTests,1), ...
    zeros(nTests,1), ...
    zeros(nTests,1), ...
    zeros(nTests,1), ...
    'VariableNames', ...
    {'Fichier', 'Duty_initial_pct', 'Duty_final_pct', ...
     'ADC_initial', 'ADC_final', 'Delta_ADC'});

all_y = {};   % sortie : angle équivalent
all_u = {};   % entrée : duty cycle normalisé

%% Lecture et traitement des essais
for k = 1:nTests

    filename = files{k,1};
    d_init   = files{k,2};
    d_final  = files{k,3};

    if ~isfile(filename)
        warning('Fichier absent : %s', filename);
        continue;
    end

    % Lecture du format : entier, texte, entier
    fid = fopen(filename, 'r');
    C = textscan(fid, '%d %s %d', 'Delimiter', ',');
    fclose(fid);

    t_ms = C{1};
    adc  = C{3};

    % Conversion en secondes
    t = t_ms / 1000;

    % Nettoyage basique
    [t, ia] = unique(t, 'stable');
    adc = adc(ia);

    valid = isfinite(t) & isfinite(adc);
    t = t(valid);
    adc = adc(valid);

    if numel(t) < 50
        warning('Pas assez de points dans %s', filename);
        continue;
    end

    % Vérification de la durée
    if abs((numel(t)*Ts) - T_record) > 5*Ts
        warning(...
            'Durée inattendue dans %s : %d points, %.1f ms', ...
            filename, numel(t), (numel(t)-1)*1000);
    end

    % Entrée : échelon de duty cycle
    % Duty normalisé entre 0 et 1
    u = d_final/100 * ones(size(t));

    % Sortie : angle équivalent en degrés
    position = (adc - ADC_0) / (ADC_100 - ADC_0);
    y = position * 90;

    % Stockage pour identification
    all_u{end+1} = u; %#ok<SAGROW>
    all_y{end+1} = y; %#ok<SAGROW>

    % Valeurs initiales et finales
    n_avg = 20;  % moyenne sur les 20 premiers et derniers ms
    ADC_init  = mean(adc(1:n_avg));
    ADC_final = mean(adc(end-n_avg+1:end));

    results.Fichier(k) = string(filename);
    results.Duty_initial_pct(k) = d_init;
    results.Duty_final_pct(k) = d_final;
    results.ADC_initial(k) = ADC_init;
    results.ADC_final(k) = ADC_final;
    results.Delta_ADC(k) = ADC_final - ADC_init;

    % Tracé
    plot(t*1000, adc, ...
        'DisplayName', sprintf('0 -> %d %%', d_final));
end

xlabel('Temps [ms]');
ylabel('ADC raw');
title('Réponses à des échelons de duty cycle');
grid on;
legend('Location', 'best');

%% Carte statique : duty final -> ADC final
figure;
plot(results.Duty_final_pct, results.ADC_final, 'o-', 'LineWidth', 1.5);
xlabel('Duty cycle final [%]');
ylabel('ADC final');
title('Caractéristique statique : duty -> ADC');
grid on;

%% Identification
% Tous les essais sont concaténés dans un objet iddata.
z = iddata(all_y, all_u, Ts);
z.InputName  = 'duty';
z.OutputName = 'angle';
z.TimeUnit   = 'seconds';

%% Modèle du premier ordre
% G1(s) = K / (tau*s + 1)
G1 = tfest(z, 1, 0);

%% Modèle du second ordre
% G2(s) = K*wn^2 / (s^2 + 2*zeta*wn*s + wn^2)
G2 = tfest(z, 2, 0);

%% Affichage
disp('--- Modèle du premier ordre ---');
G1

disp('--- Modèle du second ordre ---');
G2

%% Qualité de l'identification
figure;
compare(z, G1, G2);
title('Comparaison modèles identifiés / mesures');

%% Réponses indicielles des modèles
figure;
step(G1, 'r', 0.5);
hold on;
step(G2, 'b', 0.5);
grid on;
legend('Modèle 1er ordre', 'Modèle 2e ordre');
title('Réponse indicielle des modèles identifiés');

%% Export
writetable(results, 'resultats_echelons.csv');

G1_tf = tf(G1);
G2_tf = tf(G2);

save('modeles_papillon.mat', 'G1', 'G2', 'results', 'Ts');