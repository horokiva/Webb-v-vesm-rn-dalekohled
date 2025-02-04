# Webbův vesmírný dalekohled
Tento projekt je zaměřen na práci s maticemi a manipulaci s daty uloženými v jednorozměrném vektoru. Cílem je implementovat různé operace, které umožní čtení, úpravu a výpis matic reprezentovaných jako jednorozměrné pole. Projekt je vhodný pro studenty IT, kteří se chtějí naučit efektivně pracovat s datovými strukturami v jazyce C++.

## Popis projektu
### 1. Čtení a Parsování Matice
Projekt začíná funkcemi, které umožňují načíst matici čísel ze vstupního proudu (například ze souboru nebo standardního vstupu). Matice se nejprve analyzuje, aby se zjistily její rozměry, a poté se data ukládají do jednorozměrného vektoru.

- `parse_matrix(std::istream& in)`
  - Zjišťuje počet řádků a sloupců v matici.
  - Ověřuje, že všechny řádky mají stejný počet sloupců.
  - V případě nesouladu vyvolá výjimku `std::invalid_argument`.
- `parse_matrix(std::istream& in, const std::pair<size_t, size_t>& m_size)`

  - Načítá data matice do jednorozměrného vektoru.
  - Ověřuje, zda počet načtených prvků odpovídá zjištěné velikosti matice.
### 2. Výpis Matice
Projekt obsahuje funkci pro formátovaný výpis matice na výstup.

- `print_matrix(std::ostream& out, const std::pair<size_t, size_t>& m_size, const std::vector<int>& vec)`
  - Vypisuje matici ve formátu tabulky.
  - Používá dynamickou šířku sloupců podle nejdelšího čísla v matici.
  - Přidává rámeček okolo matice pro lepší čitelnost.
### 3. Práce s Maticí znaků (unsigned char)
Další část projektu se zaměřuje na manipulaci s maticí, která obsahuje znaky, včetně mezer.

- `parse_stream(std::istream& in, const std::pair<size_t, size_t>& m_size)`
  - Načítá znaky do jednorozměrného vektoru.
  - Ověřuje, zda počet načtených znaků odpovídá velikosti matice.
### 4. Manipulace s Maticí
Matice může být rotována a její prvky mohou být zaměněny.

- **Rotace dolů** (`rotate_down`)

  - Posune všechny řádky dolů, přičemž poslední řádek se přesune na začátek.
  - Podporuje rotaci o více kroků (i záporných).
    
- **Rotace doprava** (`rotate_right`)

  - Posune všechny sloupce doprava, přičemž poslední sloupec se přesune na začátek.
  - Podporuje rotaci o více kroků (i záporných).
    
- **Prohození dvou bodů** (`swap_points`)

  - Umožňuje výměnu dvou prvků v matici.
  - Ověřuje, že body jsou v mezích matice.
    
- **Prohození obdélníků** (`swap_points` s `delta`)

  - Výměna dvou obdélníkových oblastí v matici.
  - Kontroluje, zda se obdélníky nepřekrývají.
### 5. Dekódování Obrazu
- `decode_picture` (zatím neimplementováno)
  - Měl by umožnit načtení obrázku ze souboru a jeho transformaci do matice znaků.
