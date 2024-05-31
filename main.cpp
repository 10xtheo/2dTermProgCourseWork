#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

enum TasksTopics
{
  CREATE = 0,
  READ = 1,
  UPDATE = 2,
  DELETE = 3
};

const int MAX_STRING_SIZE = 50;
const int MAX_TASKS_COUNT = 5;
const int TASK_TOPICS_COUNT = 4;

struct Node
{
  int key;
  Node* left;
  Node* right;
};

int getNumber ()
{
  int a;
  cin >> a;
  while (cin.fail())
  {
    cin.clear();
    cin.ignore();
    cin >> a;
  }
  return a;
}

int getRandomValueFromRange(int leftLimit, int rightLimit)
{
  return leftLimit + rand() % (rightLimit - leftLimit + 1);
}

unsigned sizeTree(Node* root, unsigned size = 0)
{
  if (root == NULL) return 0;
  
  size += sizeTree(root->left, size);
  size += sizeTree(root->right, size);
  return size;
}

void printTree(Node* tree, int level = 0, bool isRight = false)
{
  if (tree == NULL) return;
  printTree(tree->right, level + 1, true);

  if (!level) cout << "-->"; 
  else cout << "   ";

  for (int i = 0; i < level; i++)
  {
    if (i + 1 == level) isRight ? cout << ".-->" : cout << "`-->";
    else cout << "    ";
  }
  
  cout << tree->key << "\n";

  printTree(tree->left, level + 1);
}

void printTreeToFile(Node* tree, ofstream& db, int level = 0, bool isRight = false)
{
  if (tree == NULL) return;
  printTreeToFile(tree->right, db, level + 1, true);

  if (!level) db << "-->";
  else db << "   ";

  for (int i = 0; i < level; i++)
  {
    if (i + 1 == level) isRight ? db << ".-->" : db << "`-->"; 
    else db << "    ";
  }
  
  db << tree->key << "\n";

  printTreeToFile(tree->left, db, level + 1);
}

void printNode(Node* node)
{
  if (node == NULL) return;
  cout << "\nNode:"
  << "\n L: " << node->left
  << "\n Key: " << node->key
  << "\n R: " << node->right
  << "\n";
}

Node* findMaxNode(Node* root)
{
  while (root->right)
  {
    root = root->right;
  }
  return root;
}

Node* findMinNode(Node* root)
{
  while (root->left)
  {
    root = root->left;
  }
  return root;
}

Node* search(int key, Node* root)
{
  if (root == NULL) return NULL;
    
  if (key == root->key) return root;
  if (key < root->key) return search(key, root->left);
  else return search(key, root->right);
  
  return NULL;
}

Node* searchParentByChild(Node* root, Node* child)
{
  if (root == NULL || child == NULL) return NULL;
    
  if (root->left == child || root->right == child) return root;
  if (child->key < root->key) return searchParentByChild(root->left, child);
  else return searchParentByChild(root->right, child);
  
  return NULL;
}

void destroyTree(Node* &root)
{
  if(root != NULL)
  {
    destroyTree(root->left);
    destroyTree(root->right);
    delete root;
    root = NULL;
  }
}

void del(Node* root, int key)
{
  Node* delNode = search(key, root);
  Node* parentNode = searchParentByChild(root, delNode);

  if (delNode == NULL) return;
  if (parentNode == NULL)
  {
    if (delNode->right != NULL && delNode->left != NULL)
    {
      Node* delChild = findMaxNode(delNode->left);
      int childKey = delChild->key;

      del(delNode, childKey);
      delNode->key = childKey;
      return;
    }

    if (delNode->right != NULL)
    {
      root = delNode->right;
      delete delNode;
      return;
    }
    else if (delNode->left != NULL)
    {
      root = delNode->right;
      delete delNode;
      return;
    }

    destroyTree(root);
    return;
  }

  if (delNode->right == NULL && delNode->left == NULL)
  {
    delNode->key < parentNode->key ? parentNode->left = NULL : parentNode->right = NULL;

    delete delNode;
    delNode = NULL;
    return;
  }
  else if (delNode->right == NULL && delNode->left != NULL)
  {
    delNode->key < parentNode->key ? parentNode->left = delNode->left : parentNode->right = delNode->left;

    delete delNode;
    delNode = NULL;
    return;
  }
  else if (delNode->right != NULL && delNode->left == NULL)
  {
    delNode->key < parentNode->key ? parentNode->left = delNode->right : parentNode->right = delNode->right;

    delete delNode;
    delNode = NULL;
    return;
  }
  else
  {
    Node* delChild = findMaxNode(delNode->left);
    int childKey = delChild->key;

    del(delNode, childKey);
    delNode->key = childKey;
    return;
  }
}

void insert(Node* &root, int key)
{ 
  if (root == NULL)
  {
    root = new Node;
    root->key = key;
    root->left = NULL; 
    root->left = NULL;
    return;
  } 

  if (key < root->key)
  {
    if (root->left != NULL) insert(root->left, key);
    else
    {
      root->left = new Node;
      root->left->key = key;
      root->left->left = NULL; 
      root->left->right = NULL;
    }
  } 
  else if (key > root->key)
  {
    if (root->right != NULL) insert(root->right, key);
    else
    {
      root->right = new Node;
      root->right->key = key;
      root->right->left = NULL;
      root->right->right = NULL;
    }
  }
}

void readTreeFromFile(Node* &root, char* filename)
{
  ifstream database(filename);
  if (!database.is_open())
  {
    cout << "\nFile with name " << filename << " doesn't exists!\n";
    return;
  }

  char data[MAX_STRING_SIZE];
  while (database.getline(data, MAX_STRING_SIZE)) insert(root, atoi(data));
      
  database.close();
  return;
  
}

void writeTreeToFile(Node* root, ofstream& db)
{
  if (!db.is_open())
  {
    cout << '\n' << "Saving error!";
  }
  else
  {
    printTreeToFile(root, db);
  }
}

void preOrderTravers(Node* root)
{ // прямой обход - сверху вниз
  if (root)
  {
    cout << root->key << " ";
    preOrderTravers(root->left);
    preOrderTravers(root->right);
  }
}

void inOrderTravers(Node* root)
{ // симм. обход - слева направо
  if (root)
  {
    inOrderTravers(root->left);
    cout << root->key << " ";
    inOrderTravers(root->right);
  }
}

void postOrderTravers(Node* root)
{ // обратный обход - снизу вверх
  if (root)
  {
    postOrderTravers(root->left);
    postOrderTravers(root->right);
    cout << root->key << " ";
  }
}


void generateTopicHead(int topic, string& head)
{
  switch (topic)
  {
    case TasksTopics::CREATE:
    {
      head = "[1] Создание бинарного дерева";
      return;
    }
    case TasksTopics::READ:
    {
      head = "\n\n[2] Поиск из значений";
      return;
    }
    case TasksTopics::UPDATE:
    {
      head = "\n\n[3]Вставка в бинарное дерево";
      return;
    }
    case TasksTopics::DELETE:
    {
      head = "\n\n[4]Удаление из бинарного дерева";
      return;
    }
  }
}


void fillVectorRandomly(vector<int>* vec, Node* root)
{
  int N = getRandomValueFromRange(5, 9);
  while ((*vec).size() < N)
  {
    int paste = getRandomValueFromRange(-99, 99);
    if (find(vec->begin(), vec->end(), paste) == vec->end() && search(paste, root) == nullptr)
      vec->push_back(paste);
  }
}

void fillTreeRandomly(Node* root)
{
  root = nullptr;
  int N = getRandomValueFromRange(5, 9);
  for (int i = 0; i < N; ++i)
    insert(root, getRandomValueFromRange(-99, 99));
}

void generateTaskCondition(int topic, string& head, Node* &taskRoot, int N1, int N2,
    vector<int> collection1, vector<int> collection2)
{

  switch (topic)
  {
    case TasksTopics::CREATE:
    {
      head = "\nСоставьте бинарное дерево поиска из чисел: ";
      for (int i = 0; i < collection1.size(); ++i)
      {
        head += to_string(collection1[i]);
        head += ' ';
      }
      head += "\n";
      return;
    }
    case TasksTopics::READ:
    {
      head = "\n Сколько элементов больше ";
      head += to_string(N1);
      head += " в дереве\n";
      return;
    }
    case TasksTopics::UPDATE:
    {
      head = "\n Вставьте числа из набора ";
      for (int i = 0; i < collection1.size(); ++i)
      {
        head += to_string(collection2[i]);
        head += ' ';
      }
      head += "в дерево\n";
      return;
    }
    case TasksTopics::DELETE:
    {
      head = "\n Удалите элементы меньше ";
      head += to_string(N2);
      head += " из дерева\n";
      return;
    }
  }
}

// обход дерева на поиск эл-в больше N
int countGreaterThanN(Node* root, int n)
{
  if (root == nullptr)
  {
      return 0;
  }
  int count = 0;
  if (root->key > n)
  {
      count += 1;
  }
  count += countGreaterThanN(root->left, n);
  count += countGreaterThanN(root->right, n);
  return count;
}

// удаление меньших N
Node* deleteNodesSmallerThanN(Node* root, int N)
{
  if (root == NULL) return NULL;
  root->left = deleteNodesSmallerThanN(root->left, N);
  root->right = deleteNodesSmallerThanN(root->right, N);
  if (root->key < N)
  {
    Node* temp = root;
    if (root->right)
    {
      root = root->right;
    }
    else
    {
      root = root->left;
    }
    delete temp;
  }
  return root;
}
void generateTaskShortAnswer(int topic, string& head, Node* &taskRoot, int N1, int N2,
    vector<int> collection1, vector<int> collection2)
{
  switch (topic)
  {
    case TasksTopics::CREATE:
    {
      head = '\n';
      head += "Ответ: \n";
      for (int i = 0; i < collection1.size(); ++i)
        insert(taskRoot, collection1[i]);
      return;
    }
    case TasksTopics::READ:
    {
      head = '\n';
      head += "Ответ: \n";
      head += "Найдено ";
      head += to_string(countGreaterThanN(taskRoot, N1));
      head += " элементов больше ";
      head += to_string(N1);
      return;
    }
    case TasksTopics::UPDATE:
    {
      head = '\n';
      for (int i = 0; i < collection2.size(); ++i)
        insert(taskRoot, collection2[i]);
      head += "Ответ: \n";
      return;
    }
    case TasksTopics::DELETE:
    {
      head = '\n';
      head += "Ответ: \n";
      head += "Дерево после удаления элементов меньше ";
      head += to_string(N2);
      head += '\n';
      taskRoot = deleteNodesSmallerThanN(taskRoot, N2);
      return;
    }
  }
}

void generateTaskAnswer(int topic, string& head, Node* &taskRoot, int N1, int N2, 
    vector<int> collection1, vector<int> collection2)
{
  switch (topic)
  {
    case TasksTopics::CREATE:
    {
      return;
    }
    case TasksTopics::READ:
    {
      return;
    }
    case TasksTopics::UPDATE:
    {
      return;
    }
    case TasksTopics::DELETE:
    {
      return;
    }
  }
}


void writeTasksInFiles(unsigned tasksCount)
{
  ofstream dbk("outputKey.txt"); // короткие ответы
  ofstream dba("outputAns.txt"); // полные ответы
  ofstream dbt("outputTask.txt"); // условия
  if (!dbt.is_open())
  {
    cout << "Ошибка файла" << endl;
    return;
  }

  string topicHead; // заголовок раздела
  string taskCondition; // условие задачи
  string taskShortAnswer; // короткий ответ на задачу
  string taskAnswer; // полный ответ на задачу
  
  Node* taskRoot = NULL;

  for (int topic = 0; topic < TASK_TOPICS_COUNT; topic++)
  { 
    // для разделов задач
    generateTopicHead(topic, topicHead);
    dbt << topicHead;
    dbk << topicHead;
    dba << topicHead;
    for (int taskIdx = 0; taskIdx < tasksCount; taskIdx++)
    {
      if (topic != 0)
      {
        for (int i = 0; i < getRandomValueFromRange(5, 7); i++) insert(taskRoot, getRandomValueFromRange(-99, 99));
      }
      // для задач внутри раздела
      int N1 = getRandomValueFromRange(-99, 99);
      int N2 = getRandomValueFromRange(-99, 99);
      vector<int> collection1;
      fillVectorRandomly(&collection1, taskRoot);
      vector<int> collection2;
      fillVectorRandomly(&collection2, taskRoot);

      generateTaskCondition(topic, taskCondition, taskRoot, N1, N2, collection1, collection2);
      dbt << taskCondition;
      if (taskRoot && topic != 0) writeTreeToFile(taskRoot, dbt);

      generateTaskShortAnswer(topic, taskShortAnswer, taskRoot, N1, N2, collection1, collection2);
      dbk << taskShortAnswer;
      if (taskRoot && topic != 1) writeTreeToFile(taskRoot, dbk);

      generateTaskAnswer(topic, taskAnswer, taskRoot, N1, N2, collection1, collection2);
      dba << taskAnswer;
      if (taskRoot) writeTreeToFile(taskRoot, dba);

      destroyTree(taskRoot);
      taskRoot = nullptr;
    }
  }
  
  dbt.close();
  dba.close();
  dbk.close();
}

int main()
{
  srand(time(0));

  Node* root = nullptr; // корень дерева


  auto start = chrono::steady_clock::now();
  auto end = chrono::steady_clock::now();

  while(true)
  {
    cout << "1. Создать новое дерево" << endl;
    cout << "2. Вывести дерево" << endl;
    cout << "3. Вставка, удаление, получение элемента дерева" << endl;
    cout << "4. Прямой, обратный, в ширину обходы бинарного дерева" << endl;
    cout << "5. Генерация заданий к практической работе по бинарным деревьям" << endl;
    cout << "Выберите пункт меню от 1 до 5 " << endl;

    int workPoint;
    workPoint = getNumber();

    switch (workPoint)
    {   
      case 1:
      {
        cout << "Заполнение дерева" << endl;
        cout << "1. Случайное заполнение дерева заданного размера" << endl;
        cout << "2. Заполнение дерева своими числами" << endl;
        cout << "3. Заполнение дерева числами из файла" << endl;

        int chooseType;
        cin >> chooseType;
        destroyTree(root);
        root = nullptr;

        switch (chooseType)
        {
          case 1:
          {
            int elementsCount;
            cout << "Введите кол-во элементов дерева: ";
            elementsCount = getNumber();
            start = chrono::steady_clock::now();
            for (int i = 0; i < elementsCount; i++) insert(root, getRandomValueFromRange(-99, 99));
            end = chrono::steady_clock::now();
            break;
          }
          case 2:
          {
            int item;
            cout << "Вводите значения для дерева через enter: " << endl;
            start = chrono::steady_clock::now();
            while (cin >> item) insert(root, item);
            end = chrono::steady_clock::now();
            break;
          }
          case 3:
          {
            char filename[MAX_STRING_SIZE];

            cout << " Введите название файла до " << MAX_STRING_SIZE << " символов, конец - /): ";
            
            cin.get();
            cin.getline(filename, MAX_STRING_SIZE, '/');
            start = chrono::steady_clock::now();
            readTreeFromFile(root, filename);
            end = chrono::steady_clock::now();
            break;
          }
          default:
          {
            cout << "Введено некорректное значение" << endl;
            break;
          }
        }
        cout << "Создано дерево:" << endl;
        printTree(root);

        cout << "Время на создание: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " mcs" << endl;

        break;
      }
      case 2:
      {
        if (root == nullptr)
        {
          cout << "Дерево пусто" << endl; 
          break;
        }
        printTree(root);
        // cout << "\n(And in tree.txt file)\n";
        ofstream db("tree.txt");
        writeTreeToFile(root, db);
        cout << "Дерево записано файл tree.txt" << endl;
        db.close();
        break;
      }
      case 3:
      {
        cout << "Выберите действие: " << endl;
        cout << "1. Получение элемента" << endl;
        cout << "2. Вставка элемента" << endl;
        cout << "3. Удаление элемента" << endl;
        int actionType; 
        actionType = getNumber();

        int data;
        cout << "Введите значение элемента: ";
        data = getNumber();

        switch (actionType)
        {
          case 1:
          {
            start = chrono::steady_clock::now();
            Node* foundedItem = search(data, root);
            end = chrono::steady_clock::now();

            if (foundedItem == nullptr) cout << "Значение не найдено" << endl;
            else cout << "Найденный элемент: " << foundedItem->key << "\n";
            
            cout << "Время поиска: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " mcs" << "\n";

            break;
          }
          case 2:
          {
            start = chrono::steady_clock::now();
            insert(root, data);
            end = chrono::steady_clock::now();
            cout << "Новое дерево:" << endl;
            printTree(root);

            cout << "Время на вставку: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " mcs" << endl;
            break;
          }
          case 3:
          {
            start = chrono::steady_clock::now();
            del(root, data);
            end = chrono::steady_clock::now();

            cout << "Новое дерево:" << endl;
            printTree(root);

            cout << "Удалено за время: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " mcs" << "\n";
            break;
          }
          default:
          {
            cout << "Некорректное значение" << endl;
            break;
          }
        }
        break;
      }
      case 4:
      {
        if (root == nullptr)
        {
          cout << "Дерево не заполнено" << endl;
          break;
        }
        
        cout << endl << "Прямой обход - сверху вниз: " << endl;
        preOrderTravers(root);
        cout << endl << "Симметричный обход - слева направо: " << endl;
        inOrderTravers(root);
        cout << endl << "Обратный обход - снизу вверх: " << endl;
        postOrderTravers(root);
        cout << endl;
        break;
      }
      case 5:
      {
        cout << "Введите число вариантов заданий для генерации: ";
        int numVar = getNumber();
        writeTasksInFiles(numVar);
        cout << endl << "Задания сгенрированы в соответствующие файлы" << endl;
        break;
      }
      default:
      {
        cout << "Неверный ввод" << endl;
        break;
      }
    }
  }
  return 0;
}
