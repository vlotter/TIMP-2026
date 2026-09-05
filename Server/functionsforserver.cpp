#include "functionsforserver.h"
#include "database.h"
#include "customsha512.h"
#include <QStringList>
#include <QDebug>
#include <QCryptographicHash>
#include <cmath>
#include <queue>
#include <vector>

QByteArray FunctionsForServer::parseAndExecute(const QByteArray& request, QString& currentUserRole) {
    QString reqStr = QString::fromUtf8(request).trimmed();
    QStringList parts = reqStr.split("&");
    if (parts.isEmpty()) return "error&empty_request\r\n";

    QString cmd = parts[0];
    if (cmd == "auth") return handleAuth(parts, currentUserRole);
    if (cmd == "reg") return handleReg(parts);
    if (cmd == "vigenere") return handleVigenere(parts);
    if (cmd == "vigenere_dec") return handleVigenereDec(parts);
    if (cmd == "sha512") return handleSha512(parts);
    if (cmd == "bisection") return handleBisection(parts);
    if (cmd == "graph") return handleGraph(parts);
    if (cmd == "admin") return handleAdmin(parts, currentUserRole);

    return "error&unknown_command\r\n";
}

QByteArray FunctionsForServer::handleAuth(const QStringList& parts, QString& role) {
    if (parts.size() < 3) return "error&invalid_params\r\n";
    if (DataBase::getInstance()->authUser(parts[1], parts[2], role)) {
        return QString("auth_ok&%1\r\n").arg(role).toUtf8();
    }
    return "auth_fail\r\n";
}

QByteArray FunctionsForServer::handleReg(const QStringList& parts) {
    if (parts.size() < 3) return "error&invalid_params\r\n";
    if (DataBase::getInstance()->registerUser(parts[1], parts[2])) {
        return "reg_ok\r\n";
    }
    return "reg_fail\r\n";
}

QByteArray FunctionsForServer::handleVigenere(const QStringList& parts) {
    if (parts.size() < 3) return "error&invalid_params\r\n";
    QString text = parts[1];
    QString key = parts[2];
    QString result = "";
    int keyIndex = 0;
    
    for (QChar c : text) {
        if (c.isLetter()) {
            int shift = key[keyIndex % key.length()].toUpper().unicode() - 'A';
            int base = c.isUpper() ? 'A' : 'a';
            result += QChar(base + (c.unicode() - base + shift) % 26);
            keyIndex++;
        } else {
            result += c;
        }
    }
    return QString("vigenere_res&%1\r\n").arg(result).toUtf8();
}

QByteArray FunctionsForServer::handleVigenereDec(const QStringList& parts) {
    if (parts.size() < 3) return "error&invalid_params\r\n";
    QString text = parts[1];
    QString key = parts[2];
    QString result = "";
    int keyIndex = 0;
    
    for (QChar c : text) {
        if (c.isLetter()) {
            int shift = key[keyIndex % key.length()].toUpper().unicode() - 'A';
            int base = c.isUpper() ? 'A' : 'a';
            result += QChar(base + (c.unicode() - base - shift + 26) % 26);
            keyIndex++;
        } else {
            result += c;
        }
    }
    return QString("vigenere_res&%1\r\n").arg(result).toUtf8();
}

QByteArray FunctionsForServer::handleSha512(const QStringList& parts) {
    if (parts.size() < 2) return "error&invalid_params\r\n";
    QString text = parts[1];
    QString hash = CustomSha512::hash(text);
    return QString("sha512_res&%1\r\n").arg(hash).toUtf8();
}

QByteArray FunctionsForServer::handleBisection(const QStringList& parts) {
    if (parts.size() < 4) return "error&invalid_params\r\n";
    // Функция для поиска корня: f(x) = x^2 - target
    double target = parts[1].toDouble();
    double a = parts[2].toDouble();
    double b = parts[3].toDouble();
    double epsilon = 0.001;
    
    auto f = [target](double x) { return x * x - target; };
    
    if (f(a) * f(b) >= 0) return "error&invalid_interval\r\n";
    
    double c = a;
    while ((b - a) >= epsilon) {
        c = (a + b) / 2;
        if (f(c) == 0.0) break;
        else if (f(c) * f(a) < 0) b = c;
        else a = c;
    }
    
    return QString("bisection_res&%1\r\n").arg(c).toUtf8();
}

QByteArray FunctionsForServer::handleGraph(const QStringList& parts) {
    // Парсинг графа и поиск кратчайшего пути
    if (parts.size() < 4) return "error&invalid_params\r\n";
    int start = parts[1].toInt();
    int end = parts[2].toInt();
    int V = parts[3].toInt();
    
    std::vector<std::vector<std::pair<int, int>>> adj(V);
    for (int i = 4; i < parts.size() - 2; i += 3) {
        int u = parts[i].toInt();
        int v = parts[i+1].toInt();
        int w = parts[i+2].toInt();
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); 
    }
    
    std::vector<int> dist(V, 1e9);
    dist[start] = 0;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    pq.push({0, start});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;
        for (auto& edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    
    if (dist[end] == 1e9) return "graph_res&unreachable\r\n";
    return QString("graph_res&%1\r\n").arg(dist[end]).toUtf8();
}

QByteArray FunctionsForServer::handleAdmin(const QStringList& parts, const QString& role) {
    if (role != "admin") return "error&access_denied\r\n";
    if (parts.size() < 3) return "error&invalid_params\r\n";
    
    QString subcmd = parts[1];
    if (subcmd == "setrole") {
        if (DataBase::getInstance()->setRole(parts[2], parts[3])) {
            return "admin_res&role_updated\r\n";
        }
    }
    return "admin_res&failed\r\n";
}
