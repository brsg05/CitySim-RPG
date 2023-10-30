#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <time.h>   
#include <string.h>

//Button GUI
typedef struct {
    bool pressed;
    Rectangle bounds;
    const char* text;
    int* constructionType;  
} Button;

#define MAX_BUTTONS 10
Button buttons[MAX_BUTTONS] = { 0 };
int numButtons = 0;

void addButton(const char* text, Rectangle bounds) {
    if (numButtons < MAX_BUTTONS) {
        buttons[numButtons].text = text;
        buttons[numButtons].bounds = bounds;
        buttons[numButtons].pressed = false;
        numButtons++;
    }
}

bool isMouseOverRectangle(Rectangle rect) {
    Vector2 mousePos = GetMousePosition();
    return (mousePos.x >= rect.x && mousePos.x <= rect.x + rect.width &&
        mousePos.y >= rect.y && mousePos.y <= rect.y + rect.height);
}

bool checkButtonPress(Button* button) {
    if (isMouseOverRectangle(button->bounds)) {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            button->pressed = true;
            return true;
        }
    }
    return false;
}

//A struct for the type of work (want to improve later on)
typedef struct {
    bool Industria;
    bool Agricultura;
    bool Informal;
    bool Desempregado;
} Trabalho;

//Struct for a Person in the city (want to improve later on)
typedef struct {
    Trabalho trabalho;
    bool Moradia;
    int Renda;
    int Idade;
    int Felicidade;
    int Revolta;
    int Sobrenome;
    int Genero;
    int Pai;
    int Mae;
} Pessoa;


//The conglomerate of persons in the city
typedef struct {
    Pessoa* pessoas; 
    int PopulacaoTotal;
    int RendaMedia;
    int FelicidadeMedia;
    int RevoltaMedia;
    int Desempregados;
    int MoradiaDesocupada;
    int IdadeMedia;
} Populacao;

//this are all types of constructions (want to improve later on)
//Every construction has a quantity in the city and a level.
typedef struct {
    int Quantidade;
    int Nivel;
}Hospital;
typedef struct {
    int Quantidade;
    int Nivel;
}Escola;
typedef struct {
    int Quantidade;
    int Nivel;
}Restaurantes;
typedef struct {
    int Quantidade;
    int Nivel;
}Esportes;
typedef struct {
    int Quantidade;
    int Nivel;
}Moradia;


//The struct for the constructions (i felt the need to add this to track the total constroctions in the city)
typedef struct {
    Hospital hospital;
    Escola escola;
    Restaurantes restaurantes;
    Esportes esportes;
    Moradia moradia;
    int TotalConstrucoes;
} Construcao;


typedef struct {
    int dia;
    int mes;
    int ano;
} Tempo;


//The struct of the city itself (want to improve later on)
typedef struct {
    Populacao populacao;
    Construcao construcao;
    int orcamento;
    Tempo tempo;
} Bairro;


//This is the function for leveling up or buy a construction.
void updateConstruction(Construcao* construcao, int constructionType) {
    switch (constructionType) {
    case 0: 
        construcao->hospital.Quantidade++;
        construcao->TotalConstrucoes++;
        break;
    case 1: 
        construcao->escola.Quantidade++;
        construcao->TotalConstrucoes++;
        break;
    case 2:
        construcao->restaurantes.Quantidade++;construcao->TotalConstrucoes++;
        break;
    case 3:
        construcao->moradia.Quantidade++;construcao->TotalConstrucoes++;
        break;
    case 4: 
        construcao->esportes.Quantidade++;construcao->TotalConstrucoes++;
        break;
    case 5: 
        construcao->hospital.Nivel++;
        break;   
    case 6: 
        construcao->escola.Nivel++;
        break;   
    case 7:  
        construcao->restaurantes.Nivel++;
        break;
    case 8:  
        construcao->moradia.Nivel++;
        break;
    case 9: 
        construcao->esportes.Nivel++;
        break;
    default: printf_s("Deu erro!!!!!");
    }
    
}

//Every person on the beginning of the city has a random job
Trabalho iniciarTrabalhoAleatorio(){
    Trabalho trabalho;
    
    int randomValue = rand() % 4;

        //Industry is 0, Agriculture is 1 and so on.
        trabalho.Industria = (randomValue % 4 == 0);
        trabalho.Agricultura = (randomValue % 4 == 1);
        trabalho.Informal = (randomValue % 4 == 2);
        trabalho.Desempregado = (randomValue % 4 == 3);

    return trabalho;
}



//Every person on the beginning of the city has random age, gender, work and housing
Pessoa iniciarPessoaAleatoria(int index) {
    Pessoa pessoa;

    pessoa.Idade = rand() % (85 - 18 + 1) + 18;
    pessoa.Felicidade = 10;
    pessoa.Revolta = 0;
    pessoa.Genero = rand() % 2; // 0 for male, 1 for female. want to acrescent other options later on.
    pessoa.trabalho = iniciarTrabalhoAleatorio();
    

    //possibility of no housing (want to work on this later on)
    //the ideia is for people with no housing to attend public houses, change the muder rate, etc.
    if (pessoa.trabalho.Desempregado) {
        pessoa.Moradia = false;
        pessoa.Renda = 0;
    }
    else {

        //every person with a job has a house (want to improve this later on)
        pessoa.Moradia = true;

        //every job has a range of salary (want to improve this later on)
        if (pessoa.trabalho.Industria) {
            pessoa.Renda = rand() % (12000 - 5000 + 1) + 5000;
        }
        else if (pessoa.trabalho.Agricultura) {
            pessoa.Renda = rand() % (8000 - 3000 + 1) + 3000;
        }
        else if (pessoa.trabalho.Informal) {
            pessoa.Renda = rand() % (1300 - 500 + 1) + 500;
        }
    }

    //Surname is equivalent of the index
    pessoa.Sobrenome = index;
    //the first ones doesnt have parents
    pessoa.Pai = 0; // Initialize to 0
    pessoa.Mae = 0; // Initialize to 0

    
    return pessoa;
}


//Inicializing the population of the city
Populacao iniciarPopulacaoAleatoria() {
    Populacao populacao;
    populacao.PopulacaoTotal = 5; // Número inicial de pessoas na população

    
    populacao.pessoas = (Pessoa*)malloc(populacao.PopulacaoTotal * sizeof(Pessoa));
    srand(time(NULL));

    for (int i = 0; i < populacao.PopulacaoTotal; i++) {
        populacao.pessoas[i] = iniciarPessoaAleatoria(i);
    }

    //initializing the information of the city
    int totalRenda = 0;
    int totalFelicidade = 0;
    int totalRevolta = 0;
    int totalDesempregados = 0;
    int totalIdade = 0;

   
    populacao.MoradiaDesocupada = 0;

    return populacao;
}

//initializing the constructions on the beginning of the city
Construcao iniciarConstrucaoAleatoria() {
    Construcao construcao;

    construcao.hospital.Quantidade = 0; 
    construcao.escola.Quantidade = 0;
    construcao.restaurantes.Quantidade = 0;
    construcao.esportes.Quantidade = 0;
    construcao.moradia.Quantidade = 0;

    construcao.hospital.Nivel = 1; 
    construcao.escola.Nivel = 1;
    construcao.restaurantes.Nivel = 1;
    construcao.esportes.Nivel = 1;
    construcao.moradia.Nivel = 1;

   
    construcao.TotalConstrucoes = construcao.hospital.Quantidade + construcao.escola.Quantidade +
        construcao.restaurantes.Quantidade + construcao.esportes.Quantidade + construcao.moradia.Quantidade;

    return construcao;
}

//this is a type of refresh on the age of the people
void atualizarIdadeEIdadeMedia(Bairro* bairro) {
    for (int i = 0; i < bairro->populacao.PopulacaoTotal; i++) {
        bairro->populacao.pessoas[i].Idade++;
    }

    int totalIdade = 0;
    for (int i = 0; i < bairro->populacao.PopulacaoTotal; i++) {
        totalIdade += bairro->populacao.pessoas[i].Idade;
    }
    if (bairro->populacao.PopulacaoTotal > 0)
        bairro->populacao.IdadeMedia = totalIdade / bairro->populacao.PopulacaoTotal;
    else bairro->populacao.IdadeMedia = 0;
}

//This is a refresh of the budget 
int atualizarOrcamento(Bairro* bairro, float taxrate) {
    int arrecadacao = 0;
    for (int i = 0; i < bairro->populacao.PopulacaoTotal; i++) {
        arrecadacao += bairro->populacao.pessoas[i].Renda * taxrate; // 25% da renda de cada pessoa
    }

    int desHos = bairro->construcao.hospital.Nivel * bairro->construcao.hospital.Quantidade * 5000;
    int desEs = bairro->construcao.escola.Nivel * bairro->construcao.escola.Quantidade * 4000;
    int desRes = bairro->construcao.restaurantes.Nivel * bairro->construcao.restaurantes.Quantidade * 1000;
    int desMor = bairro->construcao.moradia.Nivel * bairro->construcao.moradia.Quantidade * 5000;
    int desEsp = bairro->construcao.esportes.Nivel * bairro->construcao.esportes.Quantidade * 2500;
    int despesas = desHos + desEs + desRes + desMor + desEsp;

  return (arrecadacao - despesas);
}

//The time system of the city
void UpdateTime(int* days, int* months, int* years, int* frameCounter, float taxadeAtualizacao) {
    (*frameCounter)++;
    if (*frameCounter >= 144 * taxadeAtualizacao) {
        (*frameCounter) = 0;
        (*days)++;
        if (*days > 30) {
            *days = 1;
            (*months)++;
            if (*months > 12) {
                *months = 1;
                (*years)++;
            }
        }
    }
}

//pretty self-explanatory
float calculateDeathProbability(int age) {
    if (age <= 20) {
        return 0.00001; // Low probability for young people
    }
    else if (age <= 50) {
        return 0.00005; // Medium probability for middle-aged people
    }
    else {
        return 0.0002; // High probability for elderly people
    }
}


//actually removes the person
void removePersonFromPopulation(Populacao* populacao, int index) {
    // Shift elements to fill the gap
    for (int i = index; i < populacao->PopulacaoTotal - 1; i++) {
        populacao->pessoas[i] = populacao->pessoas[i + 1];
    }
    populacao->PopulacaoTotal--;

    // Resize the memory block
    populacao->pessoas = (Pessoa*)realloc(populacao->pessoas, populacao->PopulacaoTotal * sizeof(Pessoa));
}

//This initializes a newborn (want to improve this later on)
Pessoa Filho(int masc, int fem, Bairro* bairro) {
    Pessoa pessoa;


    pessoa.trabalho.Desempregado = 1;
    pessoa.trabalho.Informal = 0;
    pessoa.trabalho.Industria = 0;
    pessoa.trabalho.Agricultura = 0;
    pessoa.Genero = rand() % 2; // 0 for male, 1 for female
    pessoa.Sobrenome = masc;
    pessoa.Pai = masc; 
    pessoa.Mae = fem; 

    if (bairro->populacao.pessoas[masc].Moradia == true || bairro->populacao.pessoas[fem].Moradia == true) {
        pessoa.Moradia = true;
        pessoa.Renda = 0;
    }
    else {
        pessoa.Moradia = false;
        pessoa.Renda = 0;
    }
    
    pessoa.Idade = 0;
    pessoa.Felicidade = 10;
    pessoa.Revolta = 0;

    return pessoa;
}


//Refreshs the population
void atualizarPopulacao(Bairro* bairro, float taxRate) {

    int totalRenda = 0, totalRevolta = 0, totalIdade = 0, totalDesempregados = 0;

    if (bairro->populacao.PopulacaoTotal > 0) {
        for (int i = 0; i < bairro->populacao.PopulacaoTotal; i++) {
            Pessoa* pessoa = &bairro->populacao.pessoas[i];
            float deathProbability = calculateDeathProbability(pessoa->Idade);
            //Take the value and if it is lower then the random value, a person dies.
            if (GetRandomValue(0, 1000) < deathProbability * 1000) {
                removePersonFromPopulation(&bairro->populacao, i);
                i--;
            }
            totalRenda += bairro->populacao.pessoas[i].Renda;
            totalRevolta += bairro->populacao.pessoas[i].Revolta;
            if (bairro->populacao.pessoas[i].trabalho.Desempregado) {
                totalDesempregados++;
            }
            totalIdade += bairro->populacao.pessoas[i].Idade;
        }

        bairro->populacao.IdadeMedia = totalIdade / bairro->populacao.PopulacaoTotal;
        bairro->populacao.RendaMedia = totalRenda / bairro->populacao.PopulacaoTotal;
        bairro->populacao.RevoltaMedia = totalRevolta / bairro->populacao.PopulacaoTotal;
        bairro->populacao.Desempregados = totalDesempregados;
    }

    // Generate a new person every X days (Son or daughter)

    int i = rand() % bairro->populacao.PopulacaoTotal;
    int j = rand() % bairro->populacao.PopulacaoTotal;

    while(i == j)
         j = rand() % bairro->populacao.PopulacaoTotal;
        


    if (bairro->populacao.pessoas[i].Felicidade >= 0 && bairro->populacao.pessoas[j].Felicidade >= 0) {
        if (bairro->populacao.PopulacaoTotal < 8000) {

            Pessoa* newPessoas = (Pessoa*)malloc((bairro->populacao.PopulacaoTotal + 1) * sizeof(Pessoa));

            if (newPessoas) {
                memcpy(newPessoas, bairro->populacao.pessoas, bairro->populacao.PopulacaoTotal * sizeof(Pessoa));
                free(bairro->populacao.pessoas);
                bairro->populacao.pessoas = newPessoas;
            }

            bairro->populacao.pessoas[bairro->populacao.PopulacaoTotal] = Filho(i, j, bairro);
            bairro->populacao.PopulacaoTotal++;
        }
    }

    //Generate a new person if the city is growing (Imigrant)

    if (bairro->populacao.FelicidadeMedia > 0) {
        if (bairro->populacao.PopulacaoTotal < 8000) {
            Pessoa* novaPessoa = (Pessoa*)malloc((bairro->populacao.PopulacaoTotal + 1) * sizeof(Pessoa));

            if (novaPessoa) {
                memcpy(novaPessoa, bairro->populacao.pessoas, bairro->populacao.PopulacaoTotal * sizeof(Pessoa));
                free(bairro->populacao.pessoas);
                bairro->populacao.pessoas = novaPessoa;

                bairro->populacao.pessoas[bairro->populacao.PopulacaoTotal] = iniciarPessoaAleatoria(bairro->populacao.PopulacaoTotal);
                bairro->populacao.PopulacaoTotal++;
            }

            
        }
    }
    

    int FeliMedia = 0;
    for (int i = 0; i < bairro->populacao.PopulacaoTotal; i++) {
        Pessoa* pessoa = &bairro->populacao.pessoas[i];
        
        // Calculate happiness increment based on construction types (want to improve this later on)
        int constructionHappinessEs = bairro->construcao.escola.Quantidade * 10 * (0.2* bairro->construcao.escola.Nivel);
        int constructionHappinessHos = bairro->construcao.hospital.Quantidade * 5 * (0.2 * bairro->construcao.hospital.Nivel);
        int constructionHappinessESP = bairro->construcao.esportes.Quantidade * 2 * (0.2 * bairro->construcao.esportes.Nivel);
        int constructionHappinessRes = bairro->construcao.restaurantes.Quantidade * 1 * (0.2 * bairro->construcao.restaurantes.Nivel);
        int constructionHappinessMor = bairro->construcao.moradia.Quantidade * 1 * (0.2 * bairro->construcao.moradia.Nivel);
            
        int FeliTOTAL = constructionHappinessEs + constructionHappinessHos + constructionHappinessESP + constructionHappinessRes + constructionHappinessMor;
        

        pessoa->Felicidade += FeliTOTAL - (100*taxRate); 
        if (pessoa->Felicidade < 0) {
            pessoa->Felicidade = 0;
        }
        FeliMedia += pessoa->Felicidade;
    }
    if (bairro->populacao.PopulacaoTotal > 0)
        bairro->populacao.FelicidadeMedia = (FeliMedia / bairro->populacao.PopulacaoTotal);
    else bairro->populacao.PopulacaoTotal = 0;
}




//Slider GUI
float DrawSlider(Rectangle bounds, float minValue, float maxValue, float currentValue) {
    const int sliderWidth = 10;
    Rectangle slider = { bounds.x + (currentValue - minValue) / (maxValue - minValue) * (bounds.width - sliderWidth),
                      bounds.y, sliderWidth, bounds.height };

    DrawRectangleRec(bounds, LIGHTGRAY);
    DrawRectangleRec(slider, GRAY);

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), bounds)) {
        float mouseX = GetMouseX();
        slider.x = Clamp(mouseX - sliderWidth / 2, bounds.x, bounds.x + bounds.width - sliderWidth);
    }

    float normalizedValue = (slider.x - bounds.x) / (bounds.width - sliderWidth);
    float eu = minValue + normalizedValue * (maxValue - minValue);
    return minValue + normalizedValue * (maxValue - minValue);
}

float DrawSlider2(Rectangle bounds, float minValue, float maxValue, float currentValue) {
    const int sliderWidth = 10;
    Rectangle slider = { bounds.x + (currentValue - minValue) / (maxValue - minValue) * (bounds.width - sliderWidth),
                      bounds.y, sliderWidth, bounds.height };

    DrawRectangleRec(bounds, LIGHTGRAY);
    DrawRectangleRec(slider, GRAY);

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), bounds)) {
        float mouseX = GetMouseX();
        slider.x = Clamp(mouseX - sliderWidth / 2, bounds.x, bounds.x + bounds.width - sliderWidth);
    }

    float normalizedValue = (slider.x - bounds.x) / (bounds.width - sliderWidth);
    float eu = minValue + normalizedValue * (maxValue - minValue);
    return minValue + normalizedValue * (maxValue - minValue);
}





int main() {
   
    srand(time(NULL));
    const int screenWidth = 800;
    const int screenHeight = 600;
    Rectangle startButton = { screenWidth / 2 - 100, screenHeight / 2, 200, 50 };
    Color startButtonColor = DARKGRAY;
    float taxRate = 0.25;
    bool inGame = false;
    int days = 1;         
    int months = 1;       
    int years = 1;        
    int frameCounter = 0;
    int yearsPassed = 0;
    float taxadeAtualizacao = 0.1;
    InitWindow(screenWidth, screenHeight, "GUI Example");
    SetTargetFPS(144);
    Rectangle B1 = { 50, 50, 100, 40 };
    Rectangle B2 = { 50, 100, 100, 40 };
    Rectangle B3 = { 50, 150, 100, 40 };
    Rectangle B4 = { 50, 200, 100, 40 };
    Rectangle B5= { 50, 250, 100, 40 };
    Rectangle B6 = { 200, 50, 100, 40 };
    Rectangle B7 = { 200, 100, 100, 40 };
    Rectangle B8 = { 200, 150, 100, 40 };
    Rectangle B9 = { 200, 200, 100, 40 };
    Rectangle B10 = { 200, 250, 100, 40 };
    Rectangle slider = { 50, 400, 100, 25 };
    Rectangle slider2 = { 50, 500, 100, 25 };

    Bairro bairro;
    bairro.populacao = iniciarPopulacaoAleatoria();
    bairro.construcao = iniciarConstrucaoAleatoria();
    bairro.construcao.TotalConstrucoes = 0;
    bairro.orcamento = 1000000;
    bairro.tempo.dia = 1; // Inicializa o dia
    bairro.tempo.mes = 1; // Inicializa o mês
    bairro.tempo.ano = 1; // Inicializa o ano

    addButton("Hospital", B1);
    addButton("School", B2);
    addButton("Restaurants", B3);
    addButton("Moradias", B4);
    addButton("Esportes", B5);
    addButton("NivelHospital", B6);
    addButton("NivelEscola", B7);
    addButton("NivelRestaurante", B8);
    addButton("NivelMoradia", B9);
    addButton("NivelEsporte", B10);


    while (!WindowShouldClose()) {

        if (!inGame) {
            if (CheckCollisionPointRec(GetMousePosition(), startButton)) {
                startButtonColor = GRAY;

                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                   
                    inGame = true;
                }
            }
            else {
                startButtonColor = DARKGRAY;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        

        if (!inGame) {
            
            DrawText("Simulador do Nobre", screenWidth / 2 - MeasureText("Simulador do Nobre", 40) / 2, screenHeight / 4, 40, DARKGRAY);

           
            DrawRectangleRec(startButton, startButtonColor);
            DrawText("Start", startButton.x + 45, startButton.y + 10, 30, RAYWHITE);
        }
        else {
            ClearBackground(BLUE);
            
            taxRate = DrawSlider(slider, 0.1f, 1.0f, taxRate);
            taxadeAtualizacao = DrawSlider2(slider2, 0.000001f, 0.1f, taxadeAtualizacao);
            UpdateTime(&days, &months, &years, &frameCounter, taxadeAtualizacao);

            //every 10 days the population is refreshed
            if (days%10 == 0) {
                atualizarPopulacao(&bairro, taxRate);
            }

            //every year the population age and the budget is refreshed (want to work on this later on)
            if (years > 1) {
                atualizarIdadeEIdadeMedia(&bairro);
                bairro.orcamento += atualizarOrcamento(&bairro, taxRate);
                years = 1; // Reset years counter
                yearsPassed++;
            }

            
            //The buttons for the constructions
            for (int i = 0; i < numButtons; i++) {
                buttons[i].pressed = false;

                if (checkButtonPress(&buttons[0])) {
                    bairro.orcamento -= 100000;
                    updateConstruction(&bairro.construcao, 0);
                    break;
                }
                else
                    if (checkButtonPress(&buttons[1])) {
                        bairro.orcamento -= 50000;
                        updateConstruction(&bairro.construcao, 1);
                        break;
                    }
                    else
                        if (checkButtonPress(&buttons[2])) {
                            bairro.orcamento -= 10000;
                            updateConstruction(&bairro.construcao, 2);
                            break;
                        }
                        else
                            if (checkButtonPress(&buttons[3])) {
                                bairro.orcamento -= 10000;
                                updateConstruction(&bairro.construcao, 3);
                                break;
                            }
                            else
                                if (checkButtonPress(&buttons[4])) {
                                    bairro.orcamento -= 10000;
                                    updateConstruction(&bairro.construcao, 4);
                                    break;
                                }
                                else
                                    if (checkButtonPress(&buttons[5])) {
                                        bairro.orcamento -= 2000 * bairro.construcao.hospital.Quantidade;
                                        updateConstruction(&bairro.construcao, 5);
                                        break;
                                    }
                                    else
                                        if (checkButtonPress(&buttons[6])) {
                                            bairro.orcamento -= 2000 * bairro.construcao.escola.Quantidade;
                                            updateConstruction(&bairro.construcao, 6);
                                            break;
                                        }
                                        else
                                            if (checkButtonPress(&buttons[7])) {
                                                bairro.orcamento -= 2000 * bairro.construcao.restaurantes.Quantidade;
                                                updateConstruction(&bairro.construcao, 7);
                                                break;
                                            }
                                            else
                                                if (checkButtonPress(&buttons[8])) {
                                                    bairro.orcamento -= 2000 * bairro.construcao.moradia.Quantidade;
                                                    updateConstruction(&bairro.construcao, 8);
                                                    break;
                                                }
                                                else
                                                    if (checkButtonPress(&buttons[9])) {
                                                        bairro.orcamento -= 2000 * bairro.construcao.esportes.Quantidade;
                                                        updateConstruction(&bairro.construcao, 9);
                                                        break;
                                                    }

                if (buttons[i].pressed) {
                    DrawRectangleRec(buttons[i].bounds, LIGHTGRAY);
                }
                else if (isMouseOverRectangle(buttons[i].bounds)) {
                    DrawRectangleRec(buttons[i].bounds, GRAY);
                }
                else {
                    DrawRectangleRec(buttons[i].bounds, DARKGRAY);
                }
                DrawText(buttons[i].text, buttons[i].bounds.x + 10, buttons[i].bounds.y + 10, 10, WHITE);

            }



            //Printing on the screen (want to work on this later on)
            DrawText(TextFormat("Ano: %d, Mes: %d, Dia: %d", yearsPassed, months, days), 330, 250, 20, BLACK);
            DrawText(TextFormat("RendaMedia: %d, IdadeMedia: %d", bairro.populacao.RendaMedia, bairro.populacao.IdadeMedia), 330, 200, 20, BLACK);
            DrawText(TextFormat("Orcamento: %d", bairro.orcamento), 330, 300, 20, BLACK);
            DrawText(TextFormat("TotalContrucoes: %d", bairro.construcao.TotalConstrucoes), 330, 350, 20, BLACK);
            DrawText(TextFormat("Populacao: %d", bairro.populacao.PopulacaoTotal), 330, 400, 20, BLACK);
            DrawText(TextFormat("FelicidadeMedia: %d", bairro.populacao.FelicidadeMedia), 330, 450, 20, BLACK);
            DrawText(TextFormat("Imposto: %.0f%%",taxRate*100), 50, 450, 20, BLACK);
            DrawText(TextFormat("taxaAtualizacao: %f", taxadeAtualizacao), 50, 550, 20, BLACK);
            DrawText(TextFormat("%d", bairro.construcao.hospital.Quantidade), 170, 50, 20, BLACK);
            DrawText(TextFormat("%d", bairro.construcao.escola.Quantidade), 170, 100, 20, BLACK);
            DrawText(TextFormat("%d", bairro.construcao.restaurantes.Quantidade), 170, 150, 20, BLACK);
            DrawText(TextFormat("%d", bairro.construcao.moradia.Quantidade), 170, 200, 20, BLACK);
            DrawText(TextFormat("%d", bairro.construcao.esportes.Quantidade), 170, 250, 20, BLACK);
            DrawText(TextFormat("%d", bairro.construcao.hospital.Nivel), 310, 50, 20, BLACK);
            DrawText(TextFormat("%d", bairro.construcao.escola.Nivel), 310, 100, 20, BLACK);
            DrawText(TextFormat("%d", bairro.construcao.restaurantes.Nivel), 310, 150, 20, BLACK);
            DrawText(TextFormat("%d", bairro.construcao.moradia.Nivel), 310, 200, 20, BLACK);
            DrawText(TextFormat("%d", bairro.construcao.esportes.Nivel), 310, 250, 20, BLACK);


        }

        
        EndDrawing();
    }
    CloseWindow();
    free(bairro.populacao.pessoas);
    return 0;
}



