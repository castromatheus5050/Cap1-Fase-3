# Função para inserir dados na tabela SENSOR_DATA
def inserir_dados():
    conn, cursor = conectar_bd()
    if not conn:
        return

    try:
        # Solicitar ao usuário os dados para inserção
        humidity = float(input("Digite o valor da umidade (humidity): "))
        light_percentage = float(input("Digite o valor da porcentagem de luz (light_percentage): "))
        potassium_present = input("Potássio presente? (true/false): ").strip().lower()
        phosphorus_present = input("Fósforo presente? (true/false): ").strip().lower()

        # Comando SQL para inserção
        sql = """
        INSERT INTO SENSOR_DATA (HUMIDITY, LIGHT_PERCENTAGE, POTASSIUM_PRESENT, PHOSPHORUS_PRESENT)
        VALUES (:1, :2, :3, :4)
        """
        cursor.execute(sql, (humidity, light_percentage, potassium_present, phosphorus_present))
        conn.commit()
        print("Dados inseridos com sucesso!")

    except Exception as e:
        print("Erro ao inserir dados: ", e)
        conn.rollback()
    finally:
        cursor.close()
        conn.close()

# Função para deletar dados da tabela SENSOR_DATA
def deletar_dados():
    conn, cursor = conectar_bd()
    if not conn:
        return

    try:
        id_registro = int(input("Digite o ID do registro que deseja deletar: "))
        sql = "DELETE FROM SENSOR_DATA WHERE ID = :1"
        cursor.execute(sql, (id_registro,))
        conn.commit()

        if cursor.rowcount > 0:
            print(f"Registro com ID {id_registro} deletado com sucesso!")
        else:
            print(f"Nenhum registro encontrado com ID {id_registro}.")

    except Exception as e:
        print("Erro ao deletar dados: ", e)
        conn.rollback()
    finally:
        cursor.close()
        conn.close()

# Função para consultar dados da tabela SENSOR_DATA
def consultar_dados():
    conn, cursor = conectar_bd()
    if not conn:
        return

    try:
        sql = "SELECT * FROM SENSOR_DATA"
        cursor.execute(sql)

        colunas = [desc[0] for desc in cursor.description]
        print(f"{' | '.join(colunas)}")
        print("-" * 50)
        for row in cursor.fetchall():
            print(" | ".join(str(value) for value in row))

    except Exception as e:
        print("Erro ao consultar os dados: ", e)
    finally:
        cursor.close()
        conn.close()