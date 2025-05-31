/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/31 22:52:12 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <string.h>

static char	*get_var_name(char *str, int *i)
{
	int	start;

	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	if (str[*i] == '$')
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	if (!ft_isalnum(str[*i]) && str[*i] != '_')
		return (ft_strdup(""));
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

static char	*replace_var(char *result, char *var_name, char *var_value,
	int start)
{
	char	*before;
	char	*after;
	char	*temp1;
	char	*temp2;
	int		var_name_len;

	before = ft_substr(result, 0, start);
	var_name_len = ft_strlen(var_name);
	if (ft_strlen(var_name) == 0)
		var_name_len = 1;
	else
		var_name_len = ft_strlen(var_name) + 1;
	after = ft_strdup(result + start + var_name_len);
	if (!var_value)
		var_value = "";
	temp1 = ft_strjoin(before, var_value);
	temp2 = ft_strjoin(temp1, after);
	free(before);
	free(after);
	free(temp1);
	free(result);
	return (temp2);
}

void	process_variable(char **result, int *i, t_minishell *shell)
{
	char	*var_name;
	char	*var_value;
	int		dollar_pos;

	dollar_pos = *i;
	(*i)++;
	if (should_skip_variable(*result, *i))
		return ;
	var_name = get_var_name(*result, i);
	if (handle_invalid_var(result, var_name, dollar_pos, i))
		return ;
	var_value = get_variable_value(var_name, shell);
	*result = replace_var(*result, var_name, var_value, dollar_pos);
	if (should_free_var_value(var_name) && var_value)
		free(var_value);
	free(var_name);
	update_position(i, var_value, dollar_pos);
}

char	*expand_variables(char *str, t_minishell *shell)
{
	return (expand_variables_split(str, shell));
}
