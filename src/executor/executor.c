/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/24 18:38:40 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_ast(t_ast_node *ast, t_minishell *shell)
{
	if (!ast)
		return (0);
	if (ast->type == NODE_COMMAND)
		return (execute_command(ast, shell));
	else if (ast->type == NODE_PIPE)
		return (execute_pipeline(ast, shell));
	else if (ast->type >= NODE_REDIRECT_IN
		&& ast->type <= NODE_REDIRECT_HEREDOC)
		return (execute_redirections(ast, shell));
	return (0);
}

static int	execute_external_command(char *path, char **args, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		execve(path, args, envp);
		perror_exit("execve");
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	return (1);
}

int	execute_command(t_ast_node *node, t_minishell *shell)
{
	char	*path;
	int		exit_status;

	if (!node || !node->args || !node->args[0])
		return (0);
	if (is_builtin(node->args[0]))
		return (execute_builtin(node->args, shell));
	path = find_command_path(node->args[0], shell);
	if (!path)
	{
		print_error(node->args[0], "command not found");
		return (127);
	}
	exit_status = execute_external_command(path, node->args, shell->envp);
	free(path);
	return (exit_status);
}

static char	*search_in_paths(char **paths, char *command)
{
	char	*full_path;
	char	*temp;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, command);
		free(temp);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_command_path(char *command, t_minishell *shell)
{
	char	*path_env;
	char	**paths;
	char	*result;

	if (ft_strchr(command, '/'))
		return (ft_strdup(command));
	path_env = get_env_value("PATH", shell);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	result = search_in_paths(paths, command);
	free(paths);
	return (result);
}
