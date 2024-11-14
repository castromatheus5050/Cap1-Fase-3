# Função para conectar ao banco de dados
def conectar_bd():
    try:
        conn = oracledb.connect(user='rm560688', password="120997", dsn='oracle.fiap.com.br:1521/ORCL')
        cursor = conn.cursor()
        return conn, cursor
    except Exception as e:
        print("Erro ao conectar ao banco de dados: ", e)
        return None, None