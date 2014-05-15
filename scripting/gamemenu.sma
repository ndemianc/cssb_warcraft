#include <amxmodx>
#include <amxmisc>

#define PLUGIN "Game Menu"
#define VERSION "1.1"
#define AUTHOR "AMXMODMENU.RU"

#define GAMEMENU_FILE "resource/GameMenu.res"
#define MAX_SIZE 1012

new g_Text[MAX_SIZE]
new GameMenu[34]

public plugin_init() 
{
    register_plugin(PLUGIN, VERSION, AUTHOR)
    
    // Вызываем функцию Read_GameMenu для чтения нашего исправленного файла меню.
    set_task(0.1, "Read_GameMenu")
}

public Read_GameMenu()
{
    new i_File, s_File[128], Len, i
    
    // Получаем путь к директории с конфигами AMXX в s_File.
    get_configsdir(s_File, charsmax(s_File))
    
    // Формируем путь к gamemenu.txt файлу, используя путь к конфигам, и сохраняем в s_File.
    format(s_File, charsmax(s_File), "%s/gamemenu.txt", s_File)
    
    // Открываем файл для чтения
    i_File = fopen(s_File, "r")
    
    // Читаем содержимое файла в g_Text. Данная функция поддерживает чтение символов CR, LF.
    fgets(i_File, g_Text, MAX_SIZE)
    
    // Закрываем файл
    fclose(i_File)

    md5_file(s_File, GameMenu)
    Len = strlen(GameMenu)
    for (i = 4; i < Len-1; i++) GameMenu[i] = 0
}

public client_putinserver(id)
{
    new Params[1]
    Params[0] = id
    // Вызываем функцию Change_GameMenu
    if (is_user_bot(id)) return PLUGIN_HANDLED
    set_task(3.0, "Change_GameMenu", 0, Params[0], 1)
    return PLUGIN_HANDLED
}

public Change_GameMenu(Params[1])
{
    new id = Params[0]
    new UserGM[32]
    get_user_info(id, "_gm", UserGM, 31)
    if (!equal(UserGM, GameMenu)) {
        // Указываем путь к файлу resource/GameMenu.res
        client_cmd(id, "motdfile %s", GAMEMENU_FILE)
    
        // Записываем в файл содержимое g_Text
        client_cmd(id, "motd_write %s", g_Text)
    
        // Возвращаем значение команды по умолчанию
        client_cmd(id, "motdfile motd.txt") 

        client_cmd(id, "setinfo _gm %s", GameMenu)
    }
}

public plugin_precache()
{
    precache_generic("resource/GameMenu.tga");
    return PLUGIN_HANDLED
}