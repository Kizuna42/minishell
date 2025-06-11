/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/11 21:42:18 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <termios.h>
# include "../libft/libft.h"

# define PROMPT "minishell$ "
# define MAX_PATH 4096
# define BUFFER_SIZE 1024
# define QUOTE_NONE 0
# define QUOTE_SINGLE 1
# define QUOTE_DOUBLE 2

extern volatile sig_atomic_t	g_signal_status;
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_REDIRECT_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_SEMICOLON,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_BACKGROUND,
	TOKEN_EOF
}	t_token_type;
typedef struct s_token
{
	t_token_type		type;
	char				*value;
	int					quote_type;
	struct s_token		*next;
}	t_token;
typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIRECT_IN,
	NODE_REDIRECT_OUT,
	NODE_REDIRECT_APPEND,
	NODE_REDIRECT_HEREDOC,
	NODE_AND,
	NODE_OR,
	NODE_SEMICOLON,
	NODE_SUBSHELL,
	NODE_BACKGROUND
}	t_node_type;
typedef struct s_ast_node
{
	t_node_type			type;
	char				**args;
	char				*filename;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;
typedef struct s_minishell
{
	t_env		*env_list;
	char		**envp;
	int			last_exit_status;
	int			stdin_backup;
	int			stdout_backup;
}	t_minishell;

t_token			*tokenize(char *input);
t_token			*create_token(t_token_type type, char *value);
t_token			*create_token_with_quote(t_token_type type, char *value,
					int quote_type);
void			add_token(t_token **head, t_token *new_token);
void			free_tokens(t_token *tokens);
int				is_operator(char c);
int				is_quote(char c);
char			*extract_quoted_string(char *str, int *i);
t_token_type	get_operator_type(char *str, int *advance);
void			process_token(char *input, int *i,
					t_token **tokens, int advance);
char			*extract_word(char *str, int *i);
t_ast_node		*parse(t_token *tokens);
t_ast_node		*parse_logical_ops(t_token **tokens);
t_ast_node		*parse_pipeline(t_token **tokens);
t_ast_node		*parse_command(t_token **tokens);
t_ast_node		*parse_redirections(t_token **tokens, t_ast_node *cmd);
t_ast_node		*create_ast_node(t_node_type type);
void			free_ast(t_ast_node *ast);
int				count_args(t_token *tokens);
int				is_redirect_token(t_token_type type);
t_ast_node		*create_redirect_node(t_token **tokens);
int				count_word_tokens(t_token *tokens);
t_ast_node		*parse_mixed_command(t_token **tokens);
t_ast_node		*find_command_node(t_ast_node *node);
t_ast_node		*create_command_with_args(int arg_count);
char			**allocate_args_array(int arg_count, t_ast_node *cmd);
void			cleanup_on_error(char **args, int i, t_ast_node *cmd);
void			finalize_command_args(t_ast_node *cmd, char **args, int i);
int				validate_syntax(t_token *tokens);
void			print_syntax_error(char *token);
int				execute_ast(t_ast_node *ast, t_minishell *shell);
int				execute_command(t_ast_node *node, t_minishell *shell);
int				execute_pipeline(t_ast_node *node, t_minishell *shell);
int				execute_redirections(t_ast_node *node, t_minishell *shell);
int				execute_logical_ops(t_ast_node *node, t_minishell *shell);
char			*find_command_path(char *command, t_minishell *shell);
int				check_file_access(char *command);
int				is_builtin(char *command);
int				execute_builtin(char **args, t_minishell *shell);
int				builtin_echo(char **args);
int				builtin_cd(char **args, t_minishell *shell);
int				builtin_pwd(void);
int				builtin_export(char **args, t_minishell *shell);
int				builtin_unset(char **args, t_minishell *shell);
int				builtin_env(char **args, t_minishell *shell);
int				builtin_exit(char **args, t_minishell *shell);
t_env			*init_env(char **envp);
char			*get_env_value(char *key, t_minishell *shell);
int				set_env_value(char *key, char *value, t_minishell *shell);
int				unset_env_value(char *key, t_minishell *shell);
char			**env_to_array(t_minishell *shell);
void			free_env(t_env *env);
char			*expand_variables(char *str, t_minishell *shell);
char			**expand_args(char **args, t_minishell *shell);
char			**expand_variables_array(char **args, t_minishell *shell);
char			*expand_tilde(char *str, t_minishell *shell);
char			**expand_tilde_array(char **args, t_minishell *shell);
char			*remove_quote_markers(char *str);
void			free_args(char **args);
char			**remove_empty_args(char **args);
char			**expand_wildcard(char *pattern);
char			**expand_with_wildcards(char **args);
char			**fill_wildcard_result(char **args, char **result);
char			**split_args(char *input);
void			handle_signals(void);
void			setup_signal_handlers(void);
void			setup_child_signal_handlers(void);
void			setup_default_signal_handlers(void);
int				rl_on_new_line(void);
void			rl_redisplay(void);
void			rl_replace_line(const char *text, int clear_undo);
char			*read_input_line(void);
void			cleanup_minishell(t_minishell *shell);
int				is_whitespace(char c);
int				is_valid_identifier(char *str);
int				process_export_arg(char *arg, t_minishell *shell);
int				print_export_env(t_minishell *shell);
t_env			**create_sorted_env_array(t_minishell *shell, int *count);
char			*ft_strtok(char *str, const char *delim);
void			restore_std_fds(t_minishell *shell);
int				handle_readline_input(t_minishell *shell, char *input);
void			process_input(char *input, t_minishell *shell);
void			init_minishell(t_minishell *shell, char **envp);
int				handle_input_loop(t_minishell *shell);
char			*expand_variables_split(char *str, t_minishell *shell);
char			*extract_quoted_string_split(char *str, int *i);
void			process_variable(char **result, int *i, t_minishell *shell);
void			handle_dollar_quote(char **result, int *i);
char			*get_variable_value(char *var_name, t_minishell *shell);
int				should_free_var_value(char *var_name);
int				should_skip_variable(char *result, int i);
int				handle_input_redirect(char *filename);
int				handle_output_redirect(char *filename, int append);
int				handle_heredoc(char *delimiter, t_minishell *shell);
int				execute_redirect_list(t_ast_node **redirects, int count,
					t_minishell *shell);
int				is_redirect_node(t_ast_node *node);
t_ast_node		*collect_redirections(t_ast_node *node, t_ast_node **redirects,
					int *count);
int				read_heredoc_lines(int pipefd[2], char *delimiter,
					t_minishell *shell);
void			print_error(char *cmd, char *msg);
void			perror_exit(char *msg);
int				handle_directory_error(char *command);
int				handle_permission_error(char *command);
int				handle_cd_error(char *path);
int				handle_cd_dash(t_minishell *shell);
int				process_unset_arg(char *arg, t_minishell *shell);
int				is_valid_n_flag(char *arg);
int				handle_invalid_var(char **result, char *var_name,
					int dollar_pos, int *i);
void			update_position(int *i, char *var_value, int dollar_pos);
int				count_backslashes(char *str, int start);
void			process_backslashes(char **result, int *i);
void			handle_backslash(char **result, int *i, t_minishell *shell);
void			update_pwd_env(char *old_pwd, t_minishell *shell);
int				check_first_token(t_token *current);
int				validate_token_syntax(t_token *current);
int				check_pipe_syntax(t_token *token);
int				check_logical_syntax(t_token *token);
int				check_redirect_syntax(t_token *token);
int				is_operator_token(t_token_type type);
void			sort_matches(char **matches, int count);
int				should_match_entry(char *pattern, char *entry_name);
int				count_matches(char *pattern);
char			**fill_matches(char *pattern, int count);
void			restore_fds(int saved_stdin, int saved_stdout);
void			reset_readline_state(void);

# ifdef BONUS

t_ast_node		*parse_logical_ops(t_token **tokens);
int				execute_logical_ops(t_ast_node *node, t_minishell *shell);
char			**expand_wildcards(char *pattern);

# endif

#endif