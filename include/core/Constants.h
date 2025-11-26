#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace Constants
{
namespace TransactionStatus
{
constexpr const char *PENDING = "pending";
constexpr const char *COMPLETED = "completed";
constexpr const char *CANCELLED = "cancelled";
} 

namespace AuctionStatus
{
constexpr const char *ACTIVE = "active";
constexpr const char *COMPLETED = "completed";
constexpr const char *CANCELLED = "cancelled";
} 

namespace Messages
{
constexpr const char *NOT_FOUND = "Ничего не найдено";
constexpr const char *FOUND = "Найдено";
constexpr const char *READY = "Готов к работе";
constexpr const char *ALL_DATA_UPDATED = "Все данные обновлены";
constexpr const char *DATA_SAVED = "Данные сохранены";
constexpr const char *DATA_LOADED = "Данные загружены";
constexpr const char *SUCCESS = "Успех";
constexpr const char *ERROR = "Ошибка";
constexpr const char *INFORMATION = "Информация";
constexpr const char *CONFIRMATION = "Подтверждение";
constexpr const char *VALIDATION_ERROR = "Ошибка валидации";
} 

namespace ErrorMessages
{
constexpr const char *SYSTEM_NOT_INITIALIZED = "Система не инициализирована";
constexpr const char *UNKNOWN_ERROR = "Неизвестная ошибка";
constexpr const char *SAVE_ERROR = "Ошибка сохранения";
constexpr const char *LOAD_ERROR = "Ошибка загрузки";
constexpr const char *ADD_ERROR = "Ошибка добавления";
constexpr const char *EDIT_ERROR = "Ошибка редактирования";
constexpr const char *DELETE_ERROR = "Ошибка удаления";
} 

namespace OperationMessages
{
constexpr const char *PROPERTY_ADDED = "Недвижимость добавлена";
constexpr const char *PROPERTY_UPDATED = "Недвижимость обновлена";
constexpr const char *PROPERTY_DELETED = "Недвижимость удалена";
constexpr const char *CLIENT_ADDED = "Клиент добавлен";
constexpr const char *CLIENT_UPDATED = "Клиент обновлен";
constexpr const char *CLIENT_DELETED = "Клиент удален";
constexpr const char *TRANSACTION_ADDED = "Сделка добавлена";
constexpr const char *TRANSACTION_UPDATED = "Сделка обновлена";
constexpr const char *TRANSACTION_DELETED = "Сделка удалена";
constexpr const char *AUCTION_CREATED = "Аукцион создан";
constexpr const char *AUCTION_DELETED = "Аукцион удален";
} 

namespace SelectionMessages
{
constexpr const char *SELECT_PROPERTY_TO_EDIT = "Выберите недвижимость для редактирования";
constexpr const char *SELECT_PROPERTY_TO_DELETE = "Выберите недвижимость для удаления";
constexpr const char *SELECT_CLIENT_TO_EDIT = "Выберите клиента для редактирования";
constexpr const char *SELECT_CLIENT_TO_DELETE = "Выберите клиента для удаления";
constexpr const char *SELECT_TRANSACTION_TO_EDIT = "Выберите сделку для редактирования";
constexpr const char *SELECT_TRANSACTION_TO_DELETE = "Выберите сделку для удаления";
constexpr const char *SELECT_AUCTION_TO_VIEW = "Выберите аукцион для просмотра";
constexpr const char *SELECT_AUCTION_TO_DELETE = "Выберите аукцион для удаления";
} 

namespace ConfirmationMessages
{
constexpr const char *DELETE_PROPERTY = "Удалить недвижимость?";
constexpr const char *DELETE_CLIENT = "Удалить клиента?";
constexpr const char *DELETE_TRANSACTION = "Удалить сделку?";
constexpr const char *DELETE_AUCTION = "Удалить аукцион?";
} 

namespace InfoMessages
{
constexpr const char *NO_PROPERTIES_FOR_AUCTION = "Нет доступной недвижимости для создания аукциона";
constexpr const char *NO_PROPERTIES_FOR_TRANSACTION = "Нет доступной недвижимости для создания сделки";
constexpr const char *NO_CLIENTS_FOR_TRANSACTION = "Нет зарегистрированных клиентов для создания сделки";
constexpr const char *INSUFFICIENT_DATA = "Недостаточно данных";
constexpr const char *INSUFFICIENT_DATA_FOR_TRANSACTION = "Недостаточно данных для создания сделки";
constexpr const char *INSUFFICIENT_DATA_FOR_EDIT = "Недостаточно данных для редактирования сделки";
constexpr const char *ENTER_TRANSACTION_ID = "Введите ID сделки (6-8 цифр)";
} 

namespace DisplayStatus
{
constexpr const char *PENDING = "В ожидании";
constexpr const char *COMPLETED = "Завершена";
constexpr const char *CANCELLED = "Отменена";
constexpr const char *ACTIVE = "Активен";
constexpr const char *AUCTION_COMPLETED = "Завершен";
constexpr const char *AUCTION_CANCELLED = "Отменен";
} 

namespace MessageTimeout
{
constexpr int SHORT = 2000;
constexpr int MEDIUM = 3000;
constexpr int LONG = 5000;
} 
} 

#endif

